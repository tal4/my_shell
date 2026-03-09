#pragma once

#include <string>
#include <vector>
#include "command.h"

namespace shell {
    // Tokenize the input line into a vector of strings, handling quotes and special characters.
    std::vector<std::string> tokenize(const std::string& line);

    // Parse the command line into a Command struct, handling arguments and redirections.
    Command parse_command(const std::string& line);

}