#pragma once

#include "pipeline.h"


namespace shell {

    /**
     * Execute a pipeline of commands, connecting their input and output as needed.
     * @param pipeline The pipeline to execute
     */
    void execute_pipeline(const pipeline& pipeline);


    /**
     * Execute a single command, redirecting input and output as specified.
     * @param cmd The command to execute
     * @param input The file descriptor to use for input
     * @param output The file descriptor to use for output
     * @param background Whether the command should run in the background
     */
    void execute_command(const command& cmd, int input, int output, bool background);

    /**
     * Handles built-in commands. Returns true if the command was a built-in and was handled, false otherwise.
     * Currently supports: cd, exit
     * @param cmd The command to handle
     * @return true if the command was a built-in and was handled, false otherwise
     */
    bool handle_builtin(const command &cmd);

}