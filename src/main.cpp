#include <iostream>
#include <string>
#include "parser.h"
#include "executor.h"

int main() {
    std::string line;
    while(true) {
        std::cout << "my_shell> ";
        if (!std::getline(std::cin, line)) {        // EOF or error
            std::cout << std::endl; 
            break;
        }
        if (line == "exit") {                       // Exit command
            break;
        }
        if (line.empty()) {                        // Ignore empty lines
            continue;
        }

        Command cmd = shell::parse_command(line);
        shell::execute_command(cmd);
    }
    return 0;
}