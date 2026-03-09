#include "executor.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>


namespace shell {

    void execute_command(const Command &cmd) { 
        if (cmd.name.empty()) {
            return;
        }
        std::vector<char*> c_args;
        for (const auto& arg : cmd.args) {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }
        c_args.push_back(nullptr); 

        pid_t pid = fork();
        if (pid < 0) {                                                      // Fork failed
            perror("fork failed");
        } else if (pid == 0) {
            if (execvp(cmd.name.c_str(), c_args.data()) == -1) {            // Execution failed
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } else {
            if (!cmd.background) {
                int status;
                waitpid(pid, &status, 0);
            } else {
                std::cout << "Process " << pid << " running in background" << std::endl;
            }
        }
    }
}
