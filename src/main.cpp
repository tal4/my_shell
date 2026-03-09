#include <unistd.h>
#include <iostream>
#include <string>
#include "parser.h"
#include "executor.h"

int main() {
    char cwd[1024];
    std::string line;
    while(true) {
        std::cout << "my_shell>";
        if (getcwd(cwd, sizeof(cwd)) != nullptr)  {
            std::cout << cwd;
        }
        std::cout << "$ ";
        if (!std::getline(std::cin, line)) {        // EOF or error
            std::cout << std::endl; 
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