#pragma once

#include <vector>
#include <string>

#include "command.h"

namespace shell {
    struct pipeline {
        std::vector<command> commands;
        std::string input_file;
        std::string output_file;
        bool append = false;
        bool background = false;

        void clear() {
            commands.clear();
            input_file.clear();
            output_file.clear();
            append = false;
            background = false;
        }
    };
    
}