#pragma once

#include <string>
#include <vector>

namespace shell {
    struct command {
        std::string name;
        std::vector<std::string> args;
        int input_fd = -1;
        int output_fd = -1;

        void clear() {
            name.clear();
            args.clear();
            input_fd = -1;
            output_fd = -1;
        }
    };
}