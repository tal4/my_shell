#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "executor.h"
#include "pipeline.h"


namespace shell {
    
    void execute_pipeline(const pipeline& pip) {
        int input = STDIN_FILENO;;
        int fds[2];
        for (int i = 0; i < pip.commands.size(); i++) {
            int output = STDOUT_FILENO;
            const command& cmd = pip.commands[i];
            // Open input file for the first command.
            if (i == 0 && !pip.input_file.empty()) {
                input = open(pip.input_file.c_str(), O_RDONLY);
                if (input < 0) {
                    perror("Failed to open input file");
                    return;
                }
            }
            // Create pipe for all but the last command. 
            if (i < pip.commands.size() - 1) {
                if (pipe(fds) < 0) {
                    perror("Failed to create pipe");
                    return;
                }
                output = fds[1];
            // Open output file for the last command if specified.
            } else if (!pip.output_file.empty()) {
                int flags = O_WRONLY | O_CREAT;
                if (pip.append) {
                    flags |= O_APPEND;
                } else {
                    flags |= O_TRUNC;
                }
                output = open(pip.output_file.c_str(), flags, 0644);
                if (output < 0) {
                    perror("Failed to open output file");
                    return;
                }
            }
            execute_command(cmd, input, output, pip.background);
            // close read end of the pipe after forking the child process.
            if (input != STDIN_FILENO) {
                close(input);
            }
            // close write end of the pipe after forking the child process.
            if (output != STDOUT_FILENO) {
                close(output);
            }
            // The read end of the pipe becomes the input for the next command.
            input = fds[0]; 
        }
    }


    void execute_command(const command& cmd, int input, int output, bool background) {
        if (handle_builtin(cmd)) {
            return;
        }
        pid_t pid = fork();
        if (pid < 0) {
            perror("Failed to fork");
            return;
        } else if (pid == 0) {
            if (input != STDIN_FILENO) {
                dup2(input, STDIN_FILENO);
                close(input);
            }
            if (output != STDOUT_FILENO) {
                dup2(output, STDOUT_FILENO);
                close(output);
            }
            std::vector<char*> c_args;
            for (const auto& arg : cmd.args) {
                c_args.push_back(const_cast<char*>(arg.c_str()));
            }
            c_args.push_back(nullptr);
            execvp(cmd.name.c_str(), c_args.data());
            perror("Failed to execute command");
            exit(1);
        } else {
            if (!background) {
                waitpid(pid, nullptr, 0);
            }
        }
    }


    bool handle_builtin(const command& cmd) {
        if (cmd.name == "cd") {
            const char* path = cmd.args.size() > 1 ? cmd.args[1].c_str() : getenv("HOME");
            if (chdir(path) != 0) {
                perror("cd failed");
            }
            return true;
        }

        if (cmd.name == "exit") {
            exit(0);
        }
        
        return false;
    }
} 
