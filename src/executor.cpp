#include "executor.h"
#include "builtins.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


namespace shell {
    void execute_pipeline(const std::vector<Command>& pipeline) {
        size_t num_cmds = pipeline.size();
        int pipe_fds[2];
        int prev_fd_read_end = -1;

        for (size_t i = 0; i < num_cmds; i++) {
            if (pipeline[i].name.empty()) {
                continue;
            }

            if (!handle_builtin(pipeline[i])) {
                if (i < num_cmds - 1) {                     // Not the last command, create a pipe
                    if (pipe(pipe_fds) == -1) {   
                        perror("pipe failed");
                        return;
                    }
                }

                pid_t pid = fork();
                if (pid < 0) {                          // Fork failed
                    perror("fork failed");
                    return;
                } else if (pid == 0) {                 // child proccess
                    // redirect input
                    int input_fd = STDIN_FILENO;
                    if (prev_fd_read_end != -1) {
                        input_fd = prev_fd_read_end;
                    } else if (!pipeline[i].input_file.empty()) {
                        input_fd = open(pipeline[i].input_file.c_str(), O_RDONLY);
                        if (input_fd < 0) {
                            perror("Failed to open input file");
                            exit(EXIT_FAILURE);
                        }
                    }
                    if (input_fd != STDIN_FILENO) {
                        if (dup2(input_fd, STDIN_FILENO) < 0) {
                            perror("Failed to redirect input");
                            exit(EXIT_FAILURE);
                        }
                        close(input_fd);
                    }
                    // redirect output
                    int output_fd = STDOUT_FILENO;
                    if (i < num_cmds - 1) {
                        close(pipe_fds[0]);
                        output_fd = pipe_fds[1];
                    } else if (!pipeline[i].output_file.empty()) {
                        int flags = O_WRONLY | O_CREAT;
                        if (pipeline[i].append) {
                            flags |= O_APPEND;
                        } else {
                            flags |= O_TRUNC;
                        }
                        output_fd = open(pipeline[i].output_file.c_str(), flags, 0644);
                        if (output_fd < 0) {
                            perror("Failed to open output file");
                            exit(EXIT_FAILURE);
                        }
                    }
                    if (output_fd != STDOUT_FILENO) {
                        dup2(output_fd, STDOUT_FILENO);
                        close(output_fd);
                    }
                    // execute command
                    std::vector<char*> c_args;
                    for (const auto& arg : pipeline[i].args) {
                        c_args.push_back(const_cast<char*>(arg.c_str()));
                    }
                    c_args.push_back(nullptr);
                    if (execvp(pipeline[i].name.c_str(), c_args.data()) == -1) {
                        perror("execvp failed");
                        exit(EXIT_FAILURE);
                    }
                }
                if (prev_fd_read_end != -1) {
                    close(prev_fd_read_end);
                }
                if (i < num_cmds - 1) {
                    close(pipe_fds[1]);
                    prev_fd_read_end = pipe_fds[0];
                }
                if (pid > 0) {
                    if (!pipeline[i].background) {
                        int status;
                        waitpid(pid, &status, 0);
                    } else {
                        std::cout << "[Background process: " << pid << "]\n";
                    }
                }
            }
        }
    }
} 
