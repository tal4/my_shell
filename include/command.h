#pragma once

#include <string>
#include <vector>

struct Command {
    std::string name;
    std::vector<std::string> args;

    std::string input_file;
    std::string output_file;
    bool append = false;

    bool background = false;

    void clear() {
        name.clear();
        args.clear();
        input_file.clear();
        output_file.clear();
        append = false;
        background = false;
    }
};