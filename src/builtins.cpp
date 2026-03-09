#include "builtins.h"
#include <unistd.h>
#include <iostream>

namespace shell {
    bool handle_builtin(const Command& cmd) {
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