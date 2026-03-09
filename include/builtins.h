#pragma once
#include "command.h"

namespace shell {

    // Checks if the command is a built-in command and executes it if so. Returns true if the command was handled as a built-in, false otherwise.
    bool handle_builtin(const Command &cmd);  
}