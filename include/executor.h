#pragma once
#include "command.h"

namespace shell {

    // Execute the given Command struct.
    void execute_command(const Command& cmd);

}