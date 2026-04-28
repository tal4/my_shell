#include <unistd.h>
#include <iostream>
#include <string>
#include <signal.h>
#include <termios.h>


#include "main.h"
#include "parser.h"
#include "executor.h"

int main() {
    signal(SIGINT, SIG_IGN); // auto-reap background children
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
        if (line.empty()) {                        // Ignore empty line. 
            continue;
        }
        // Handle multi-line input. 
        while (line.back() == '\\') {
            line.pop_back();
            std::string next_line;
            if (!std::getline(std::cin, next_line)) {
                break;
            }
            if (next_line.empty()) {
                break;
            }
            line += (" " + next_line);
        }
        auto commands = shell::parse_command(line);
        shell::execute_pipeline(commands);
    }
    return 0;
}

void set_raw_mode(bool enable) {
    static struct termios oldt;
    struct termios newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}