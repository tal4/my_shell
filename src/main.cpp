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

        auto tokens = shell::tokenize(line);

        std::cout << "Tokens:" << std::endl;
        for (const auto& token : tokens) {
            std::cout << "[" << token << "] ";
        }
        std::cout << "\n";
    }
    return 0;
}