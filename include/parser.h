#pragma once

#include <string>
#include <vector>
#include "command.h"

namespace shell {
    std::vector<std::string> tokenize(const std::string& line);
    Command parse_command(const std::string& line);
}