#include "parser.h"
#include <sstream>
#include <vector>
#include <string>

namespace shell {
    std::vector<std::string> tokenize(const std::string& line) {
            std::vector<std::string> tokens;
            std::string current;
            bool in_quotes = false;
            
        for (char c : line) {
            if (c == '#') {  // Handle comments: ignore the rest of the line after '#'
                break;
            }
            if (c == '"') {                 // Toggle in_quotes when we encounter a double quote
                in_quotes = !in_quotes;
            } else if (std::isspace(c) && !in_quotes) {         // Handle whitespace as token separators only when not in quotes
                if(!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
            } else if ((c == '>' || c == '<' || c == '&' || c == '|') && !in_quotes) {      // Handle special characters as separate tokens
                if (current == ">" && c == '>') {               // Handle '>>' as a single token  
                    tokens.push_back(">>");
                    current.clear();
                } else if (!current.empty()) {                  // Push the current token before the special character
                    tokens.push_back(current);
                    tokens.push_back(std::string(1, c));
                    current.clear();                   
                } else {
                    tokens.push_back(std::string(1, c));
                }
            } else {                    // Regular character, add to current token
                current += c;  
            }
        }
        if (!current.empty()) {         // Push any remaining token after the loop
            tokens.push_back(current);
        }
        return tokens;
    }
}