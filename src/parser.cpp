#include "parser.h"
#include <sstream>
#include <vector>
#include <string>

namespace shell {

    std::vector<std::string> tokenize(const std::string& line) {
            std::vector<std::string> tokens;
            std::string current;
            bool in_quotes = false;
            
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '#' && !in_quotes) {  // Handle comments: ignore the rest of the line after '#'
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
                if (!current.empty()) {                  // Push the current token before the special character
                    tokens.push_back(current);
                    current.clear();
                }
                if (c == '>' && i + 1 < line.size() && line[i + 1] == '>') {  // Handle '>>' as a single token
                    tokens.push_back(">>");
                    i++;  
                } else {
                    tokens.push_back(std::string(1, c));  // Push the special character as a token
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

    Command parse_command(const std::string &line)
    {
        Command cmd;
        auto tokens = tokenize(line);
        if (tokens.empty()) {
            return cmd;
        }
        cmd.name = tokens[0];

        for (size_t i  = 0; i < tokens.size(); i++) {
            if (tokens[i] == ">") {
                if (i + 1 < tokens.size()) {
                    cmd.output_file = tokens[++i];
                }
            } else if (tokens[i] == ">>") {
                if (i + 1 < tokens.size()) {
                    cmd.output_file = tokens[++i];
                    cmd.append = true;
                }
            } else if (tokens[i] == "<") {
                if (i + 1 < tokens.size()) {
                    cmd.input_file = tokens[++i];
                }
            } else if (tokens[i] == "&") {
                cmd.background = true;
            } else {
                cmd.args.push_back(tokens[i]);
            }
        }
        return cmd;
    }
}