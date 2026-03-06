#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "parser.h"

void test_tokenize_basic() {
    std::string input = "ls -l > out.txt";
    std::vector<std::string> expected = {"ls", "-l", ">", "out.txt"};
    
    std::vector<std::string> result = shell::tokenize(input);
    
    assert(result == expected);
    std::cout << "Test Basic Tokenize: PASSED" << std::endl;
}

void test_tokenize_quotes() {
    std::string input = "echo \"hello world\"";
    std::vector<std::string> expected = {"echo", "hello world"};
    
    std::vector<std::string> result = shell::tokenize(input);
    
    assert(result == expected);
    std::cout << "Test Quotes Tokenize: PASSED" << std::endl;
}

int main() {
    test_tokenize_basic();
    test_tokenize_quotes();
    std::cout << "\nAll tests passed successfully!" << std::endl;
    return 0;
}