#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlCHandler(int sig_num) {

    cout << "smash: got ctrl-C" << endl;
    SmallShell &smash = SmallShell::getInstance();
    pid_t process = smash.getFG();
    if (process == -1) {
        return;
    }
    if (kill(process, SIGKILL) == -1) {
        perror("smash error: kill failed");
        return;
    }
    smash.setNewFG(nullptr, -1);
    std::cout << "smash: process " << process << " was killed" << std::endl;
}

void ctrlZHandler(int sig_num) {

    cout << "smash: got ctrl-Z" << endl;
    SmallShell &smash = SmallShell::getInstance();
    pid_t process = smash.getFG();
    if (process == -1) {
        return;
    }
    if(!smash.isFGinJobsList()) {
        smash.getJobsList()->addNewJob(smash.getFGcmdpt(), process, true);
    }
    if (kill(process, SIGSTOP) == -1) {
        perror("smash error: kill failed");
        return;
    }
    std::cout << "smash: process " << process << " was stopped" << std::endl;
}

void alarmHandler(int sig_num) {
    // TODO: Add your implementation
}
