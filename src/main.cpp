#include <iostream>
#include <string>
#include "parser.h"

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
        std::cout << "Command name: " << cmd.name << std::endl;
        std::cout << "Arguments: ";
        for (const auto& arg : cmd.args) {
            std::cout << "[" << arg << "] ";
        }
        std::cout << std::endl;
        std::cout << "Input file: " << (cmd.input_file.empty() ? "None" : cmd.input_file) << std::endl;
        std::cout << "Output file: " << (cmd.output_file.empty() ? "None" : cmd.output_file) << std::endl;
        std::cout << "Append: " << (cmd.append ? "Yes" : "No") << std::endl;
        std::cout << "Background: " << (cmd.background ? "Yes" : "No") << std::endl;
    }
    return 0;
}