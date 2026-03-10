#include "executor.h"
#include "builtins.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


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

        if(!handle_builtin(cmd)) {                              // Not a built-in command, execute as external command
            pid_t pid = fork();
            if (pid < 0) {                                                      // Fork failed
                perror("fork failed");
            } else if (pid == 0) {                                              // Child process
                if (!cmd.input_file.empty())  {
                    int fd = open(cmd.input_file.c_str(), O_RDONLY);
                    if (fd < 0) {
                        perror("Failed to open input file");
                        exit(EXIT_FAILURE);
                    }
                    if (dup2(fd, STDIN_FILENO) < 0) {
                        perror("Failed to redirect input");
                        exit(EXIT_FAILURE);
                    }
                    close(fd);
                }
                if (!cmd.output_file.empty()) {
                    int flags = O_WRONLY | O_CREAT;
                    if (cmd.append) {
                        flags |= O_APPEND;
                    } else {
                        flags |= O_TRUNC;
                    }
                    int fd = open(cmd.output_file.c_str(), flags, 0644);
                    if (fd < 0) {
                        perror("Failed to open output file");
                        exit(EXIT_FAILURE);
                    }
                    if (dup2(fd, STDOUT_FILENO) < 0) {
                        perror("Failed to redirect output");
                        exit(EXIT_FAILURE);
                    }
                    close(fd);
                }
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
}
