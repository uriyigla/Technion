
#include <vector>
#include "Queue.h"


int main() {

Queue<std::vector<char>> q;
std::vector<char> v1={'0','A'};
q.pushBack(v1);
    std::vector<char> v2={'6','7'};
    char b=q.front()[0];
    b=q.front()[1];

    Queue<std::vector<char>> q1(q2),q2;
    q2=q1;
    q2.front()=v2;
    char c=q2.front()[1];

     c=q2.front()[0];
int lem=2;
}

