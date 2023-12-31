#include <unistd.h>
#include <string.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include <cctype>
#include <sched.h>
#include "Commands.h"
#include <sys/stat.h>
#include <string>
#include <cerrno>
#include <fcntl.h>
#include <algorithm>

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

string _ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
    return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
    FUNC_ENTRY()
    int i = 0;
    std::istringstream iss(_trim(string(cmd_line)).c_str());
    for(std::string s; iss >> s; ) {
        args[i] = (char*)malloc(s.length()+1);
        memset(args[i], 0, s.length()+1);
        strcpy(args[i], s.c_str());
        args[++i] = NULL;
    }
    return i;

    FUNC_EXIT()
}

void _freeArgs(char **args, int len) {
    for (int i = 0; i<len ; i++) {
        free(args[i]);
    }
}

bool _isBackgroundComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return str[str.find_last_not_of(WHITESPACE)] == '&';
}

bool _isPipeComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return (str.find("|") != std::string::npos);
}

bool _isPipeAndComamnd(const char* cmd_line) {
    const string str(cmd_line);

    return (str.find("|&") != std::string::npos);
}

bool _isRedirectionComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return (str.find(">") != std::string::npos);
}

bool _isDoubleRedirectionComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return (str.find(">>") != std::string::npos);
}

bool _isQuestionMarkComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return (str.find("?") != std::string::npos);
}

bool _isStarMarkComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return (str.find("*") != std::string::npos);
}

void _removeBackgroundSign(char* cmd_line) {
    const string str(cmd_line);
    // find last character other than spaces
    unsigned int idx = str.find_last_not_of(WHITESPACE);
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // if the command line does not end with & then return
    if (cmd_line[idx] != '&') {
        return;
    }
    // replace the & (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = ' ';
    // truncate the command line string up to the last non-space character
    cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

bool isNumber(const std::string &maybenum) {
    std::string::const_iterator it = maybenum.begin();
    while (it != maybenum.end() && (std::isdigit(*it) || *it == '-')) ++it;
    return !maybenum.empty() && it == maybenum.end();
}

Command::Command(const char* input) : pid(-1), timeout(nullptr), isExternal(false)
{
    this->original_cmd_line = new char[strlen(input) + 1];
    std::strcpy((this->original_cmd_line), const_cast<char*>(input));
    this->cmd_line = new char[strlen(input) + 1];
    std::strcpy((this->cmd_line), const_cast<char*>(input));
    _removeBackgroundSign(this->cmd_line);
    this->isBackground = _isBackgroundComamnd(input);
}

Command::~Command() {
    delete[] this->cmd_line;
    delete[] this->original_cmd_line;
}

char* Command::getCmd() {
    return cmd_line;
}

char* Command::getOriginalCmd() {
    return original_cmd_line;
}

pid_t Command::getCMDPid() {
    return this->pid;
}

bool Command::isExternalCmd() {
    return this->isExternal;
}

bool Command::isBackgroundCmd() {
    return this->isBackground;
}

BuiltInCommand::BuiltInCommand(const char* cmd_line) : Command(cmd_line) {} // no fork according to instruction

ChangePromptCommand::ChangePromptCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {}

void ChangePromptCommand::execute() {
    char* words[COMMAND_MAX_ARGS];
    int len = _parseCommandLine(this->cmd_line, words);
    if (len == 1) {
        SmallShell::getInstance().setPrompt("smash");
    }
    else {
        SmallShell::getInstance().setPrompt(words[1]);
    }
    _freeArgs(words, len);
}

ShowPidCommand::ShowPidCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {}

void ShowPidCommand::execute() {
    int shall_pid = SmallShell::getInstance().getPid();
    std::cout << "smash pid is " << shall_pid << std::endl;
}

GetCurrDirCommand::GetCurrDirCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {}

void GetCurrDirCommand::execute() {
    char buffer[COMMAND_ARGS_MAX_LENGTH + 1];
    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        perror("getcwd() error");
    } else {
        std::cout << buffer << std::endl;
    }
}

ChangeDirCommand::ChangeDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

void ChangeDirCommand::execute() {
    char* args[COMMAND_ARGS_MAX_LENGTH];
    int num_of_args = _parseCommandLine(this->cmd_line, args);
    if (num_of_args > 2) {
        std::cerr << "smash error: cd: too many arguments" << std::endl;
        return;
    }
    SmallShell &smash = SmallShell::getInstance();
    if (num_of_args == 1) {
        return;
    }
    char curr_path[COMMAND_ARGS_MAX_LENGTH + 1];
    if (getcwd(curr_path, sizeof(curr_path)) == nullptr) {
        perror("getcwd() error");
        return;
    }
    char* new_path;
    if (std::strcmp("-", args[1]) == 0){
        if (smash.getLastPath() == nullptr) {
            std::cerr << "smash error: cd: OLDPWD not set" << std::endl;
            return;
        }
        new_path = smash.getLastPath();
    }
    else {
        new_path = args[1];
    }
    if (chdir(new_path) == -1) {
        perror("smash error: chdir failed");
        return;
    }
    smash.setLastPath(curr_path);
}

JobsCommand::JobsCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

void JobsCommand::execute() {
    SmallShell::getInstance().getJobsList()->printJobsList();
}

ForegroundCommand::ForegroundCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

void ForegroundCommand::execute() {
    char* args[COMMAND_ARGS_MAX_LENGTH];
    int num_of_args = _parseCommandLine(this->cmd_line, args);
    if (num_of_args > 2) {
        std::cerr << "smash error: fg: invalid arguments" << std::endl;
        return;
    }
    JobsList::JobEntry* to_the_fg;
    int id;
    SmallShell &smash = SmallShell::getInstance();
    if (num_of_args == 1) {
        to_the_fg = smash.getJobsList()->getLastJob();
        if (to_the_fg == nullptr) {
            std::cerr << "smash error: fg: jobs list is empty" << std::endl;
            return;
        }
        id = to_the_fg->getJobId();
    } else {
        if (isNumber(args[1])) {
            id = stoi(args[1]);
            to_the_fg = smash.getJobsList()->getJobById(id);
            if (to_the_fg == nullptr) {
                std::cerr << "smash error: fg: job-id " << id << " does not exist" << std::endl;
                return;
            }
        } else {
            std::cerr << "smash error: fg: invalid arguments" << std::endl;
            return;
        }
    }
    if(!to_the_fg->isNotStopped()) {
        if(kill(to_the_fg->getPid(),SIGCONT) == -1){
            perror("smash error: kill failed");
            return;
        }
        to_the_fg->setStopped(false);
    }
    std::cout << to_the_fg->getCommand()->getOriginalCmd() << " : " << to_the_fg->getPid() << std::endl;
    smash.setNewFG(to_the_fg->getCommand(), to_the_fg->getPid(), true);
    int status;
    int status_wait = waitpid(to_the_fg->getPid(), &status, WUNTRACED);
    if ((status_wait == to_the_fg->getPid()) || (status_wait == -1)){
        if (WIFSTOPPED(status)) { // check if sigSTOP(ctrl+z) was sent to son procces
            to_the_fg->setStopped(true);
            smash.setNewFG(nullptr, -1);
        } else { // check if
            smash.getJobsList()->removeJobById(id);
            smash.setNewFG(nullptr, -1);
        }
        if (status_wait == -1) {
            perror("smash error: waitpid failed");
            smash.getJobsList()->removeJobById(id);
            smash.setNewFG(nullptr, -1);
        }
        return;
    }
    if(status_wait != to_the_fg->getPid()) {
        smash.getJobsList()->removeJobById(id);
    }
    smash.setNewFG(nullptr, -1);
}

BackgroundCommand::BackgroundCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

void BackgroundCommand::execute() {
    char* args[COMMAND_ARGS_MAX_LENGTH];
    int num_of_args = _parseCommandLine(this->cmd_line, args);
    if (num_of_args > 2) {
        std::cerr << "smash error: bg: invalid arguments" << std::endl;
        return;
    }
    JobsList::JobEntry* to_resume;
    int id;
    SmallShell &smash = SmallShell::getInstance();
    if (num_of_args == 1) {
        to_resume = smash.getJobsList()->getLastStoppedJob(&id);
        if (to_resume == nullptr) {
            std::cerr << "smash error: bg: there is no stopped jobs to resume" << std::endl;
            return;
        }
    }
    else {
        id = stoi(args[1]);
        to_resume = smash.getJobsList()->getJobById(id);
        if (to_resume == nullptr) {
            std::cerr << "smash error: bg: job-id "<< id << " does not exist" << std::endl;
            return;
        }
    }
    if(!to_resume->isNotStopped()) {
        std::cout << to_resume->getOriginalCMD() << " : " << to_resume->getPid()<< std::endl;
        if(kill(to_resume->getPid(),SIGCONT) == -1){
            perror("smash error: kill failed");
            return;
        }
        to_resume->setStopped(false);
    } else {
        std::cerr << "smash error: bg: job-id " << id << " is already running in the background" <<std::endl;
    }
}

QuitCommand::QuitCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

void QuitCommand::execute() {
    char* args[COMMAND_ARGS_MAX_LENGTH];
    int number_of_args = _parseCommandLine(this->cmd_line, args);
    SmallShell::getInstance().getJobsList()->removeFinishedJobs();
    if ((number_of_args == 2) && (std::strcmp(args[1], "kill") == 0) ){
        std::cout << "smash: sending SIGKILL signal to " << SmallShell::getInstance().getJobsList()->getSize() << " jobs:" << std::endl;
        SmallShell::getInstance().getJobsList()->printJobsListKill();
        SmallShell::getInstance().getJobsList()->killAllJobs();
    }
    exit(0);
}

KillCommand::KillCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

void KillCommand::execute() {
    char* args[COMMAND_ARGS_MAX_LENGTH];
    int number_of_args = _parseCommandLine(this->cmd_line, args);
    if(number_of_args != 3) {
        std::cerr << "smash error: kill: invalid arguments" <<std::endl;
        return;
    }
    if(!isNumber(args[1]) || !isNumber(args[2])) {
        std::cerr << "smash error: kill: invalid arguments" <<std::endl;
        return;
    }
    int jobID = stoi(args[2]);
    int sig_num = stoi(args[1]) * (-1);
    JobsList::JobEntry* job = SmallShell::getInstance().getJobsList()->getJobById(jobID);
    if (job == nullptr) {
        std::cerr << "smash error: kill: job-id "<< args[2] <<" does not exist" <<std::endl;
        return;
    }
    if (jobID < 1 || sig_num < 0 || sig_num > 64){
        std::cerr << "smash error: kill: invalid arguments" <<std::endl;
        return;
    }
    if (kill(job->getPid(), sig_num) == -1) {
        perror("smash error: kill failed");
        return;
    }
    std::cout << "signal number "<< sig_num <<" was sent to pid "<< job->getPid() << std::endl;
    if (sig_num == SIGCONT) {
        job->setStopped(false);
    }
    if (sig_num == SIGSTOP) {
        job->setStopped(true);
    }
    if (sig_num == SIGKILL) {
        SmallShell::getInstance().getJobsList()->removeJobById(jobID);
    }
}

ExternalCommand::ExternalCommand(const char* cmd_line) : Command(cmd_line)  {
    this->isExternal = true;
}

void ExternalCommand::execute() {
    if(_isQuestionMarkComamnd(this->cmd_line) || _isStarMarkComamnd(this->cmd_line)) {
        char *const args[] = {"/bin/bash", "-c", this->cmd_line, NULL};
        execvp(args[0], args);
        perror("smash error: execvp failed");
        exit(-1);
    } else {
        char* args[COMMAND_ARGS_MAX_LENGTH];
        int number_of_args = _parseCommandLine(this->cmd_line, args);
        execvp(args[0], args);
        perror("smash error: execvp failed");
        exit(-1);
    }
}

PipeCommand::PipeCommand(const char *cmd_line) : Command(cmd_line), cmd_to_pipe(nullptr), cmd_from_pipe(nullptr) {
    this->change_std_error = _isPipeAndComamnd(original_cmd_line);
    std::string spliceBy;
    if (_isPipeAndComamnd(original_cmd_line)) {
        spliceBy = "|&";
    } else {
        spliceBy = "|";
    }
    std::string cmd = this->original_cmd_line;
    size_t pos = cmd.find(spliceBy);
    std::string left = cmd.substr(0, pos);
    std::string right = cmd.substr(pos + spliceBy.length());
    const char* cmd_to = left.c_str();
    const char* cmd_from= right.c_str();
    this->cmd_from_pipe = SmallShell::getInstance().CreateCommand(cmd_from);
    this->cmd_to_pipe = SmallShell::getInstance().CreateCommand(cmd_to);
}

void PipeCommand::execute() {
    pid_t from_pipe_pid = fork();
    if(from_pipe_pid > 0) {
        int status;
        int status_wait = waitpid(pid, &status, WUNTRACED);
        if (status_wait == -1) {
            perror("smash error: waitpid failed");
            return;
        }
    } else if (from_pipe_pid < 0) {
        perror("smash error: fork failed");
        return;
    }
    if (from_pipe_pid == 0) {
        if(setpgrp() < 0) {
            perror("smash error: setpgrp failed");
        }
        int fd[2];
        if((pipe(fd) == -1)) {
            perror("smash error: pipe failed");
        }
        pid_t to_pipe_pid = fork();
        if (to_pipe_pid < 0) {
            perror("smash error: fork failed");
            exit(1);
        }
        if (to_pipe_pid == 0) {
            if (setpgrp() < 0) {
                perror("smash error: setpgrp failed");
                exit(1);
            }
            if (dup2(fd[1], STDOUT_FILENO) == -1) {
                perror("smash error: dup2 failed");
                exit(1);
            }
            if (this->change_std_error) {
                if (dup2(fd[1], STDERR_FILENO) == -1) {
                    perror("smash error: dup2 failed");
                    exit(1);
                }
            }
            if (close(fd[0]) == -1) {
                perror("smash error: close failed");
                exit(1);
            }
            cmd_to_pipe->execute();
            exit(0);
        }
        if (to_pipe_pid > 0) {
            if (dup2(fd[0], STDIN_FILENO) == -1) {
                perror("smash error: dup2 failed");
                exit(1);
            }
            if (close(fd[1]) == -1) {
                perror("smash error: close failed");
                exit(1);
            }
            if (to_pipe_pid > 0) {
                int status_wait = waitpid(pid, NULL, WUNTRACED);
                if (status_wait == -1) {
                    perror("smash error: waitpid failed");
                    exit(1);
                }
            }
            cmd_from_pipe->execute();
            exit(0);
        }
    }
}

RedirectionCommand::RedirectionCommand(const char *cmd_line) : Command(cmd_line) {
    this->is_double_arrow = _isDoubleRedirectionComamnd(cmd_line);
    std::string spliceBy;
    if (_isDoubleRedirectionComamnd(original_cmd_line)) {
        spliceBy = ">>";
    } else {
        spliceBy = ">";
    }
    std::string cmd_ = this->original_cmd_line;
    size_t pos = cmd_.find(spliceBy);
    std::string left = cmd_.substr(0, pos);
    std::string right = cmd_.substr(pos + spliceBy.length());
    right.erase(std::remove_if(right.begin(), right.end(), ::isspace), right.end());
    const char* cmd_char = left.c_str();
    this->cmd = SmallShell::getInstance().CreateCommand(cmd_char);
    this->file =  new char[right.length() + 1];
    std::strcpy(this->file, right.c_str());
}

RedirectionCommand::~RedirectionCommand() {
    delete[] this->file;
}

void RedirectionCommand::execute() {
    pid_t pid = fork();
    if (pid > 0) {
        int status;
        int status_wait = waitpid(pid, &status, WUNTRACED);
        if (status_wait == -1) {
            perror("smash error: waitpid failed");
            return;
        }
    }
    if (pid < 0) {
        perror("smash error: fork failed");
    }
    if (pid == 0) {
        if(setpgrp() < 0) {
            perror("smash error: setpgrp failed");
        }
        int fd;
        if(this->is_double_arrow) {
            fd = open(this->file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        } else {
            fd = open(this->file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        }
        if (fd < 0) {
            perror("smash error: open failed");
            exit(1);
        }
        if(dup2(fd, STDOUT_FILENO) == -1) {
            perror("smash error: dup2 failed");
            exit(1);
        }
        this->cmd->execute();
        if(close(fd) < 0){
            perror("smash error: close failed");
        }
        exit(0);
    }
}

SetcoreCommand::SetcoreCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

bool checkValidCore (char* c){
    int num = std::stoi(c);
    int k =sysconf(_SC_NPROCESSORS_CONF);
    if (k < 0) {
        perror("Smash Error: sysconf Failed");
    }
    if ( num < k){
        return true;
    }
    return false;
}

void SetcoreCommand::execute(){
    char* args[COMMAND_ARGS_MAX_LENGTH];
    int number_of_args = _parseCommandLine(this->cmd_line, args);
    if (number_of_args != 3){
        std::cerr << "smash error: setcore: invalid arguments"<<std::endl;
        return;
    }
    int num;
    try {
        num = std::stoi(args[1]);
    }
    if (SmallShell::getInstance().getJobsList()->getJobById(num) == nullptr){
        std::cerr << "smash error: setcore: job-id "<< args[1] <<" does not exist" <<std::endl;
        return;
    }
    if (!checkValidCore (args[2])){
        std::cerr << "smash error: setcore: invalid core number"<<std::endl;
        return;
    }
    int change_core_pid = SmallShell::getInstance().getJobsList()->getJobById(num)->getPid();
    int core_number = std::stoi(args[2]);
    cpu_set_t mask;
    CPU_ZERO(&mask);
    if (CPU_SET(core_number, &mask) == -1) {
        perror("smash error: CPU_SET failed");
        return;
    }
    int result = sched_setaffinity(change_core_pid, sizeof(mask), &mask);   // Set the CPU affinity
    if (result == -1) {
        perror("smash error: sched_setaffinity failed");
    }
    return;
}

GetFileTypeCommand::GetFileTypeCommand(const char *cmd_line): BuiltInCommand(cmd_line) {}

void GetFileTypePrint(char* name, char* type, int size) {
    std::cout << name << "'s type is \"" << type << "\" and takes up to " << size << " bytes." << std::endl;
}

void GetFileTypeCommand::execute() {
    char* args[COMMAND_ARGS_MAX_LENGTH];
    int number_of_args = _parseCommandLine(this->cmd_line, args);

    if(number_of_args != 2) {
        std::cerr << "smash error: gettype: invalid arguments" << std::endl;
        return;
    }

    struct stat st;
    if (stat(args[1], &st) == -1) {
        perror("smash error: stat failed");
        return ;
    }
    std::size_t fileSize = st.st_size;

    if (S_ISREG(st.st_mode)) {
        GetFileTypePrint(args[1],"regular file",fileSize);
        return;
    }
    if (S_ISDIR(st.st_mode)) {
        GetFileTypePrint(args[1],"directory",fileSize);
        return;
    }
    if (S_ISCHR(st.st_mode)) {
        GetFileTypePrint(args[1],"character device",fileSize);
        return;
    }
    if (S_ISBLK(st.st_mode)) {
        GetFileTypePrint(args[1],"block device",fileSize);
        return;
    }
    if (S_ISFIFO(st.st_mode)) {
        GetFileTypePrint(args[1],"FIFO",fileSize);
        return;
    }
    if (S_ISLNK(st.st_mode)) {
        GetFileTypePrint(args[1],"symbolic link",fileSize);
        return;
    }
    if (S_ISSOCK(st.st_mode)) {
        GetFileTypePrint(args[1],"socket",fileSize);
        return;
    }
    return;
}

ChmodCommand::ChmodCommand(const char *cmd_line): BuiltInCommand(cmd_line) {}

void ChmodCommand::execute() {
    char* args[COMMAND_ARGS_MAX_LENGTH];
    int number_of_args = _parseCommandLine(this->cmd_line, args);
    if(number_of_args != 3) {
        std::cerr << "smash error: chmod: invalid arguments" <<std::endl;
        return;
    }
    mode_t mode = strtol(args[1], nullptr, 8);
    if (chmod(args[2], mode) != 0) {
        perror("smash error: chmod failed");
    }
    return;
}

SmallShell::SmallShell() : smash_prompt("smash> "), current_fg_pid(-1), current_fg_cmd(nullptr),
                           last_path(nullptr), jobs(new JobsList()), is_FG_inJB(false){
    smash_pid = getpid();
    if (smash_pid == -1)
        perror("smash error: getpid failed");
}

SmallShell::~SmallShell() {
    delete jobs;
    if (this->last_path != nullptr) {
        delete[] this->last_path;
    }
}

void SmallShell::setPrompt(std::string new_prompt) {
    this->smash_prompt = new_prompt + "> ";
}

pid_t SmallShell::getPid() {
    return this->smash_pid;
}

char* SmallShell::getLastPath() {
    return this->last_path;
}

void SmallShell::setLastPath(char* path) {
    if(this->last_path == nullptr) {
        this->last_path = new char[COMMAND_ARGS_MAX_LENGTH];
    }
    strcpy(this->last_path, path);
}

JobsList* SmallShell::getJobsList() {
    return this->jobs;
}

std::string SmallShell::getPrompt() {
    return this->smash_prompt;
}

pid_t SmallShell::getFG() {
    return this->current_fg_pid;
}

Command* SmallShell::getFGcmdpt() {
    return this->current_fg_cmd;
}

bool SmallShell::isFGinJobsList() {
    return this->is_FG_inJB;
}

Command * SmallShell::CreateCommand(const char* cmd_line) {

    string cmd_s = _trim(string(cmd_line));
    string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));
    if(_isDoubleRedirectionComamnd(cmd_line) || _isRedirectionComamnd(cmd_line)) {
        return new RedirectionCommand(cmd_line);
    }
    if (_isPipeAndComamnd(cmd_line) || _isPipeComamnd(cmd_line)) {
        return new PipeCommand(cmd_line);
    }

    if (firstWord.compare("chprompt") == 0) {  //chprompt
        return new ChangePromptCommand(cmd_line);
    }
    if (firstWord.compare("showpid") == 0) { //showpid
        return new ShowPidCommand(cmd_line);
    }
    if (firstWord.compare("pwd") == 0) { // pwd
        return new GetCurrDirCommand(cmd_line);
    }
    if (firstWord.compare("cd") == 0) { //cd
        return new ChangeDirCommand(cmd_line);
    }
    if (firstWord.compare("jobs") == 0) { // jobs
        return new JobsCommand(cmd_line);
    }
    if (firstWord.compare("fg") == 0) { //fg
        return new ForegroundCommand(cmd_line);
    }
    if (firstWord.compare("bg") == 0) { // bg
        return new BackgroundCommand(cmd_line);
    }
    if (firstWord.compare("quit") == 0) { // quit
        return new QuitCommand(cmd_line);
    }
    if (firstWord.compare("kill") == 0) { // kill
        return new KillCommand(cmd_line);
    }
    if (firstWord.compare("setcore") == 0) { // setcore
        return new SetcoreCommand(cmd_line);
    }
    if (firstWord.compare("getfiletype") == 0) { // getfiletype
        return new GetFileTypeCommand(cmd_line);
    }
    if (firstWord.compare("chmod") == 0) { // chmod
        return new ChmodCommand(cmd_line);
    }
    return new ExternalCommand(cmd_line);
    return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
    if((std::strlen(cmd_line) == 0) || (std::string(cmd_line) == "\n")) {
        return;
    }
    SmallShell &smash = SmallShell::getInstance();
    smash.getJobsList()->removeFinishedJobs();
    Command* cmd = CreateCommand(cmd_line);
    if (cmd->isExternalCmd()) {
        pid_t c_pid = fork();
        if (c_pid < 0) {
            perror("smash error: fork failed");
        }
        if (c_pid == 0) {
            if (setpgrp() < 0) {
                perror("smash error: setpgrp failed");
            }
            cmd->execute();
        } else {
            if (!cmd->isBackgroundCmd()) {
                smash.setNewFG(cmd, c_pid);
                int status;
                int status_wait = waitpid(c_pid, &status, WUNTRACED);
                if (status_wait == -1) {
                    if (!(WIFSTOPPED(status))) {
                        perror("smash error: waitpid failed");
                        return;
                    }
                }
                smash.setNewFG(nullptr, -1);
            } else {
                smash.getJobsList()->removeFinishedJobs();
                smash.getJobsList()->addNewJob(cmd, c_pid);
            }
        }
    } else if (cmd != nullptr) {
        cmd->execute();
        delete cmd;
    }
}

void SmallShell::setNewFG(Command* current_fg_cmd1, pid_t current_fg_pid1, bool is_in_bj) {
    this->current_fg_cmd = current_fg_cmd1;
    this->current_fg_pid = current_fg_pid1;
    this->is_FG_inJB = is_in_bj;
}

JobsList::JobsList() : max_ID(1) {
    this->jobsList = new std::vector<JobsList::JobEntry*>();
}

JobsList::~JobsList(){
    for (auto i = jobsList->begin(); i != jobsList->end(); ++i ) {
        delete *i;
    }
    delete jobsList;
}

JobsList::JobEntry::JobEntry(int id, Command *given_cmd,pid_t c_pid, bool stopped) :
        job_id(id), cmd(given_cmd),pid(c_pid), isStopped(stopped) {;
    if(time(&this->creation_time) == ((time_t) - 1)){
        perror("smash error: time failed");
        return;
    }
}

JobsList::JobEntry::~JobEntry() {
    if (this->cmd != nullptr) {
        delete cmd;
    }
}

pid_t JobsList::JobEntry::getPid() {
    return this->pid;
}

int JobsList::JobEntry::getJobId() {
    return this->job_id;
}

char* JobsList::JobEntry::getCMD() {
    return this->cmd->getCmd();
}

char* JobsList::JobEntry::getOriginalCMD()  {
    return this->cmd->getOriginalCmd();
}

time_t JobsList::JobEntry::getCreationTime(){
    return this->creation_time;
}

bool JobsList::JobEntry::isNotStopped() {
    return !(this->isStopped);
}

Command* JobsList::JobEntry::getCommand(){
    return this->cmd;
}

void JobsList::JobEntry::setStopped(bool val) {
    this->isStopped = val;
}

bool JobsList::JobEntry::isFinished() {
    int result = waitpid(this->getPid(), nullptr, WNOHANG);
    if (result > 0) {
        return true;
    } else if (result == 0) {
        return false;
    }
    return false;
}

void JobsList::addJob(JobsList::JobEntry* new_job) {
    if (jobsList->empty()) {
        jobsList->push_back(new_job);
    } else {
        for (vector<JobsList::JobEntry *>::iterator it = jobsList->begin(); it < jobsList->end(); it++) {
            if((*it)->getJobId() > new_job->getJobId()){
                jobsList->insert(it - 1, new_job);
                break;
            }
        }
    }
}

void JobsList::addNewJob(Command* cmd, pid_t c_pid, bool isStopped) {
    removeFinishedJobs();
    JobEntry* new_job = new JobEntry(this->getNewID(), cmd, c_pid, isStopped);
    jobsList->push_back(new_job);
}

void JobsList::removeFinishedJobs() {
    for (vector<JobsList::JobEntry *>::iterator it = jobsList->begin(); it < jobsList->end(); it++) {
        int status;
        int result = waitpid((*it)->getPid(), &status, WNOHANG);
        if (result > 0) {
            delete *it;
            it = jobsList->erase(it);
        }
    }
}

void JobsList::printJobsList() {
    removeFinishedJobs();
    for (vector<JobsList::JobEntry *>::iterator it = jobsList->begin(); it < jobsList->end(); it++) {
        time_t time_diff = time(nullptr);
        if (time_diff == (time_t)-1){
            perror("smash error: time failed");
        }
        std::cout << "[" << (*it)->getJobId() << "] ";
        std::cout << (*it)->getOriginalCMD() << " : " << (*it)->getPid() << " ";
        std::cout << difftime(time_diff, (*it)->getCreationTime()) << " secs";
        if(!((*it)->isNotStopped())) {
            std::cout << " (stopped)";
        }
        std::cout << std::endl;
    }
}

void JobsList::printJobsListKill() {
    removeFinishedJobs();
    for (vector<JobsList::JobEntry *>::iterator it = jobsList->begin(); it < jobsList->end(); it++) {
        std::cout << (*it)->getPid() << ": ";
        std::cout << (*it)->getOriginalCMD();
        std::cout << std::endl;
    }
}

JobsList::JobEntry* JobsList::getJobById(int jobId) {
    for (vector<JobsList::JobEntry *>::iterator it = jobsList->begin(); it < jobsList->end(); it++) {
        if ((*it)->getJobId() == jobId) {
            return (*it);
        }
    }
    return nullptr;
}

void JobsList::removeJobById(int jobId) {
    for (vector<JobsList::JobEntry*>::iterator it = jobsList->begin(); it < jobsList->end(); it++){
        if ((*it)->getJobId() == jobId) {
            jobsList->erase(it);
            return;
        }
    }
}

JobsList::JobEntry* JobsList::getLastJob() {
    if (this->jobsList->empty()) {
        return nullptr;
    }
    return (jobsList->back());
}

JobsList::JobEntry* JobsList::getLastStoppedJob(int* lastJobId) {
    for (vector<JobsList::JobEntry *>::reverse_iterator it = jobsList->rbegin(); it < jobsList->rend(); it++){
        if (!(*it)->isNotStopped()) {
            JobEntry* last_job =  (*it);
            *lastJobId = last_job->getJobId();
            return last_job;
        }
    }
    return nullptr;
}

int JobsList::getSize() {
    return jobsList->size();
}

int JobsList::getNewID() {
    if(this->jobsList->empty()) {
        return 1;
    }
    JobsList::JobEntry * last = jobsList->back();
    return (last->getJobId()+1);
}

void JobsList::killAllJobs() {
    for (vector<JobsList::JobEntry *>::iterator it = jobsList->begin(); it < jobsList->end(); it++) {
        if (kill((*it)->getPid(), SIGKILL) == -1) {
            perror("smash error: kill failed");
            return;
        }
    }
}