#pragma once
#include "command.h"

namespace shell {

    // Execute a pipeline of commands, connecting their input and output as needed.
    void execute_pipeline(const std::vector<Command>& pipeline);

}