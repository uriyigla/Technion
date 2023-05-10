
#include <functional>

#include "TestUtils.h"

namespace HealthPointsTests {
	bool testInitialization();
	bool testArithmaticOperators();
	bool testComparisonOperators();
	bool testOutputOperator();
}

namespace QueueTests {
	bool testQueueMethods();
	bool testModuleFunctions();
	bool testExceptions();
	bool testConstQueue();
}

std::function<bool()> testsList[] = {
	HealthPointsTests::testInitialization,
	HealthPointsTests::testArithmaticOperators,
	HealthPointsTests::testComparisonOperators,
	HealthPointsTests::testOutputOperator,

	QueueTests::testQueueMethods,
	QueueTests::testModuleFunctions,
	QueueTests::testExceptions,
	QueueTests::testConstQueue
};

const int NUMBER_OF_TESTS = sizeof(testsList)/sizeof(std::function<bool()>);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		for (int i = 0; i < NUMBER_OF_TESTS; ++i) {
		runTest(testsList[i], "Test " + std::to_string(i + 1));
		}
	}
	else {
		int idx = -1;
		try {
		idx = std::stoi(argv[1]) - 1;
		} catch (const std::invalid_argument &e) {
		std::cerr << "Error: invalid argument: " << argv[1] << std::endl;
		return 1;
		}
		if (idx < 0 || idx > NUMBER_OF_TESTS - 1) {
		std::cerr << "Error: index out of range: " << argv[1] << std::endl;
		return 1;
		}
		runTest(testsList[idx], "Test " + std::to_string(idx + 1));
	}
	return 0;
}
