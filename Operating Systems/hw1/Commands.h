#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>


#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

class SmallShell;

class Command {
protected:
    char* cmd_line; //user input
    char* original_cmd_line;
    pid_t pid;
    char* timeout;
    bool isBackground;
    bool isExternal;

public:
    Command(const char* input);
    virtual ~Command();
    virtual void execute() = 0;

    char* getCmd();
    char* getOriginalCmd();
    pid_t getCMDPid();
    bool isExternalCmd();
    bool isBackgroundCmd();
};

class BuiltInCommand : public Command {
public:
    BuiltInCommand(const char* cmd_line);
    virtual ~BuiltInCommand() {}
};

class ChangePromptCommand : public BuiltInCommand {
public:
    explicit ChangePromptCommand(const char* cmd_line);
    virtual ~ChangePromptCommand() {}
    void execute() override;
};

class ExternalCommand : public Command {
public:
    ExternalCommand(const char* cmd_line);
    virtual ~ExternalCommand() {}
    void execute() override;
};

class PipeCommand : public Command {
    bool change_std_error;
    Command* cmd_to_pipe;
    Command* cmd_from_pipe;

public:
    PipeCommand(const char* cmd_line);
    virtual ~PipeCommand() {}
    void execute() override;
};

class RedirectionCommand : public Command {
    bool is_double_arrow;
    Command* cmd;
    char* file;

public:
    explicit RedirectionCommand(const char* cmd_line);
    ~RedirectionCommand() override;
    void execute() override;
};

class ChangeDirCommand : public BuiltInCommand {
public:
    ChangeDirCommand(const char* cmd_line);
    virtual ~ChangeDirCommand() {}
    void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand(const char* cmd_line);
    virtual ~GetCurrDirCommand() {}
    void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand(const char* cmd_line);
    virtual ~ShowPidCommand() {}
    void execute() override;
};

class JobsList;

class QuitCommand : public BuiltInCommand {
public:
    QuitCommand(const char* cmd_line);
    virtual ~QuitCommand() {}
    void execute() override;
};

class JobsCommand : public BuiltInCommand {
public:
    JobsCommand(const char* cmd_line);
    virtual ~JobsCommand() {}
    void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
public:
    ForegroundCommand(const char* cmd_line);
    virtual ~ForegroundCommand() {}
    void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
public:
    BackgroundCommand(const char* cmd_line);
    virtual ~BackgroundCommand() {}
    void execute() override;
};

class TimeoutCommand : public BuiltInCommand {
    /* Bonus */
    // TODO: Add your data members
public:
    explicit TimeoutCommand(const char* cmd_line);
    virtual ~TimeoutCommand() {}
    void execute() override;
};

class ChmodCommand : public BuiltInCommand {
private:
public:
    ChmodCommand(const char* cmd_line);
    virtual ~ChmodCommand() = default;
    void execute() override;
};

class GetFileTypeCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    GetFileTypeCommand(const char* cmd_line);
    virtual ~GetFileTypeCommand() {}
    void execute() override;
};

class SetcoreCommand : public BuiltInCommand {
public:
    SetcoreCommand(const char* cmd_line);
    virtual ~SetcoreCommand() {}
    void execute() override;
};

class KillCommand : public BuiltInCommand {
public:
    KillCommand(const char* cmd_line);
    virtual ~KillCommand() {}
    void execute() override;
};

class JobsList {
public:
    class JobEntry {
        int job_id;
        pid_t pid;
        Command* cmd;
        bool isStopped;
        time_t creation_time;

    public:
        JobEntry(int id, Command* given_cmd, pid_t c_pid, bool stopped);
        ~JobEntry();
        int getJobId();
        pid_t getPid();
        char* getCMD();
        char* getOriginalCMD();
        time_t getCreationTime();
        bool isNotStopped();
        Command* getCommand();
        void setStopped(bool val);
        bool isFinished();
    };

private:
    std::vector<JobsList::JobEntry*>* jobsList;
    int max_ID;

public:
    JobsList();
    ~JobsList();
    void addNewJob(Command* cmd, pid_t c_pid, bool isStopped = false);
    void addJob(JobEntry* Job);
    void printJobsList();
    void printJobsListKill();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry* getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry* getLastJob();
    JobEntry* getLastStoppedJob(int* jobId);
    int getSize();
    int getNewID();
};

class SmallShell {
private:
    pid_t smash_pid;
    std::string smash_prompt;
    pid_t current_fg_pid;
    Command* current_fg_cmd;
    char* last_path;
    JobsList* jobs;
    bool is_FG_inJB;

    SmallShell();

public:
    Command* CreateCommand(const char* cmd_line);
    SmallShell(SmallShell const&) = delete;
    void operator=(SmallShell const&) = delete;
    static SmallShell& getInstance()
    {
        static SmallShell instance;
        return instance;
    }
    ~SmallShell();
    void executeCommand(const char* cmd_line);
    void setPrompt(std::string new_prompt);
    pid_t getPid();
    char* getLastPath();
    void setLastPath(char* path);
    JobsList* getJobsList();
    void setNewFG(Command* current_fg_cmd, pid_t current_fg_pid, bool in_fg = false);
    std::string getPrompt();
    pid_t getFG();
    Command* getFGcmdpt();
    bool isFGinJobsList();
};

#endif //SMASH_COMMAND_H_
