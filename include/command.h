#pragma once

#include <string>
#include <vector>

namespace shell {
    struct command {
        std::string name;
        std::vector<std::string> args;

        void clear() {
            name.clear();
            args.clear();
        }
    };
}