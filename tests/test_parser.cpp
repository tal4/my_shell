#include "parser.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace shell;

// Test empty input
TEST(TokenizeTest, EmptyInput) {
    std::vector<std::string> tokens = tokenize("");
    EXPECT_TRUE(tokens.empty());
}

// Test simple command with no spaces
TEST(TokenizeTest, SimpleCommand) {
    std::vector<std::string> tokens = tokenize("ls");
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0], "ls");
}

// Test command with arguments separated by spaces
TEST(TokenizeTest, CommandWithArgs) {
    std::vector<std::string> tokens = tokenize("ls -la /home");
    EXPECT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], "-la");
    EXPECT_EQ(tokens[2], "/home");
}

// Test multiple spaces between tokens
TEST(TokenizeTest, MultipleSpaces) {
    std::vector<std::string> tokens = tokenize("ls   -la   /home");
    EXPECT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], "-la");
    EXPECT_EQ(tokens[2], "/home");
}

// Test leading and trailing spaces
TEST(TokenizeTest, LeadingTrailingSpaces) {
    std::vector<std::string> tokens = tokenize("  ls -la  ");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], "-la");
}

// Test output redirection operator (>)
TEST(TokenizeTest, OutputRedirection) {
    std::vector<std::string> tokens = tokenize("ls > output.txt");
    EXPECT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], ">");
    EXPECT_EQ(tokens[2], "output.txt");
}

// Test append redirection operator (>>)
TEST(TokenizeTest, AppendRedirection) {
    std::vector<std::string> tokens = tokenize("echo hello >> log.txt");
    EXPECT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "hello");
    EXPECT_EQ(tokens[2], ">>");
    EXPECT_EQ(tokens[3], "log.txt");
}

// Test input redirection operator (<)
TEST(TokenizeTest, InputRedirection) {
    std::vector<std::string> tokens = tokenize("cat < input.txt");
    EXPECT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "cat");
    EXPECT_EQ(tokens[1], "<");
    EXPECT_EQ(tokens[2], "input.txt");
}

// Test background operator (&)
TEST(TokenizeTest, BackgroundOperator) {
    std::vector<std::string> tokens = tokenize("sleep 10 &");
    EXPECT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "sleep");
    EXPECT_EQ(tokens[1], "10");
    EXPECT_EQ(tokens[2], "&");
}

// Test pipe operator (|)
TEST(TokenizeTest, PipeOperator) {
    std::vector<std::string> tokens = tokenize("ls | grep test");
    EXPECT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], "|");
    EXPECT_EQ(tokens[2], "grep");
    EXPECT_EQ(tokens[3], "test");
}

// Test quoted string without spaces
TEST(TokenizeTest, QuotedStringNoSpaces) {
    std::vector<std::string> tokens = tokenize("echo \"hello\"");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "hello"); // Quotes should be removed
}

// Test quoted string with spaces
TEST(TokenizeTest, QuotedStringWithSpaces) {
    std::vector<std::string> tokens = tokenize("echo \"hello world\"");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "hello world"); // Spaces preserved inside quotes
}

// Test multiple quoted strings
TEST(TokenizeTest, MultipleQuotedStrings) {
    std::vector<std::string> tokens = tokenize("echo \"first\" \"second\" \"third\"");
    EXPECT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "first");
    EXPECT_EQ(tokens[2], "second");
    EXPECT_EQ(tokens[3], "third");
}

// Test mixed quoted and unquoted arguments
TEST(TokenizeTest, MixedQuotedUnquoted) {
    std::vector<std::string> tokens = tokenize("echo test \"hello world\" again");
    EXPECT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "test");
    EXPECT_EQ(tokens[2], "hello world");
    EXPECT_EQ(tokens[3], "again");
}

// Test special characters inside quotes (should be treated as regular chars)
TEST(TokenizeTest, SpecialCharsInQuotes) {
    std::vector<std::string> tokens = tokenize("echo \"test>file<back\"");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "test>file<back"); // > and < should not be treated as operators
}

// Test comment handling (everything after # is ignored)
TEST(TokenizeTest, CommentHandling) {
    std::vector<std::string> tokens = tokenize("ls -la # this is a comment");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], "-la");
}

// Test comment at the beginning
TEST(TokenizeTest, CommentAtBeginning) {
    std::vector<std::string> tokens = tokenize("# this is a full comment line");
    EXPECT_TRUE(tokens.empty());
}

// Test hash inside quotes (should not be treated as comment)
TEST(TokenizeTest, HashInQuotes) {
    std::vector<std::string> tokens = tokenize("echo \"test#comment\"");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "test#comment");
}

// Test redirection with quoted filename
TEST(TokenizeTest, QuotedRedirectionFile) {
    std::vector<std::string> tokens = tokenize("echo hello > \"my output.txt\"");
    EXPECT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "hello");
    EXPECT_EQ(tokens[2], ">");
    EXPECT_EQ(tokens[3], "my output.txt");
}

// Test complex command with multiple special chars
TEST(TokenizeTest, ComplexCommand) {
    std::vector<std::string> tokens = tokenize("cat < input.txt | grep test > output.txt &");
    EXPECT_EQ(tokens.size(), 9);
    EXPECT_EQ(tokens[0], "cat");
    EXPECT_EQ(tokens[1], "<");
    EXPECT_EQ(tokens[2], "input.txt");
    EXPECT_EQ(tokens[3], "|");
    EXPECT_EQ(tokens[4], "grep");
    EXPECT_EQ(tokens[5], "test");
    EXPECT_EQ(tokens[6], ">");
    EXPECT_EQ(tokens[7], "output.txt");
    EXPECT_EQ(tokens[8], "&");
}

// Test consecutive special characters
TEST(TokenizeTest, ConsecutiveSpecialChars) {
    std::vector<std::string> tokens = tokenize("ls > | &");
    EXPECT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], ">");
    EXPECT_EQ(tokens[2], "|");
    EXPECT_EQ(tokens[3], "&");
}

// Test only whitespace
TEST(TokenizeTest, OnlyWhitespace) {
    std::vector<std::string> tokens = tokenize("   \t\n  ");
    EXPECT_TRUE(tokens.empty());
}

// Test single quote character (should be treated as regular char since we only handle double quotes)
TEST(TokenizeTest, SingleQuotes) {
    std::vector<std::string> tokens = tokenize("echo 'hello world'");
    EXPECT_EQ(tokens.size(), 3); // Single quotes are not handled, so it's one token with quotes
    EXPECT_EQ(tokens[0], "echo");
    // Note: 'hello and world would be separate tokens if we only handle double quotes
}

// Test escaped quote (if supported by tokenizer - currently not implemented)
TEST(TokenizeTest, EscapedQuote) {
    std::vector<std::string> tokens = tokenize("echo \"test \\\" quote\"");
    // Depending on implementation, this might be different
    // Currently our tokenizer doesn't handle escape sequences
}

// Test double quotes as separate tokens when not paired
TEST(TokenizeTest, UnpairedQuotes) {
    std::vector<std::string> tokens = tokenize("echo \"hello world");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "hello world"); // Quote starts but doesn't end, rest is treated as one token
}

// Test multiple redirection operators
TEST(TokenizeTest, MultipleRedirections) {
    std::vector<std::string> tokens = tokenize("ls > out.txt >> app.txt < in.txt");
    EXPECT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], ">");
    EXPECT_EQ(tokens[2], "out.txt");
    EXPECT_EQ(tokens[3], ">>");
    EXPECT_EQ(tokens[4], "app.txt");
    EXPECT_EQ(tokens[5], "<");
    EXPECT_EQ(tokens[6], "in.txt");
}

// Test empty input
TEST(ParseCommandTest, EmptyInput) {
    Command cmd = parse_command("");
    EXPECT_TRUE(cmd.args.empty());
    EXPECT_TRUE(cmd.input_file.empty());
    EXPECT_TRUE(cmd.output_file.empty());
    EXPECT_FALSE(cmd.append);
    EXPECT_FALSE(cmd.background);
}

// Test simple command with no arguments
TEST(ParseCommandTest, SimpleCommandNoArgs) {
    Command cmd = parse_command("ls");
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(cmd.args[0], "ls"); // Command name is now at args[0]
    EXPECT_TRUE(cmd.input_file.empty());
    EXPECT_TRUE(cmd.output_file.empty());
    EXPECT_FALSE(cmd.append);
    EXPECT_FALSE(cmd.background);
}

// Test command with arguments
TEST(ParseCommandTest, CommandWithArgs) {
    Command cmd = parse_command("ls -la /home");
    EXPECT_EQ(cmd.args.size(), 3);
    EXPECT_EQ(cmd.args[0], "ls"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "-la");
    EXPECT_EQ(cmd.args[2], "/home");
    EXPECT_TRUE(cmd.input_file.empty());
    EXPECT_TRUE(cmd.output_file.empty());
    EXPECT_FALSE(cmd.append);
    EXPECT_FALSE(cmd.background);
}

// Test output redirection (>)
TEST(ParseCommandTest, OutputRedirection) {
    Command cmd = parse_command("ls > output.txt");
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(cmd.args[0], "ls"); // Command name at args[0]
    EXPECT_TRUE(cmd.input_file.empty());
    EXPECT_EQ(cmd.output_file, "output.txt");
    EXPECT_FALSE(cmd.append);
    EXPECT_FALSE(cmd.background);
}

// Test append redirection (>>)
TEST(ParseCommandTest, AppendRedirection) {
    Command cmd = parse_command("echo hello >> log.txt");
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "echo"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "hello");
    EXPECT_TRUE(cmd.input_file.empty());
    EXPECT_EQ(cmd.output_file, "log.txt");
    EXPECT_TRUE(cmd.append);
    EXPECT_FALSE(cmd.background);
}

// Test input redirection (<)
TEST(ParseCommandTest, InputRedirection) {
    Command cmd = parse_command("cat < input.txt");
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(cmd.args[0], "cat"); // Command name at args[0]
    EXPECT_EQ(cmd.input_file, "input.txt");
    EXPECT_TRUE(cmd.output_file.empty());
    EXPECT_FALSE(cmd.append);
    EXPECT_FALSE(cmd.background);
}

// Test background execution (&)
TEST(ParseCommandTest, BackgroundExecution) {
    Command cmd = parse_command("sleep 10 &");
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "sleep"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "10");
    EXPECT_TRUE(cmd.input_file.empty());
    EXPECT_TRUE(cmd.output_file.empty());
    EXPECT_FALSE(cmd.append);
    EXPECT_TRUE(cmd.background);
}

// Test combination of redirections and background
TEST(ParseCommandTest, ComplexRedirection) {
    Command cmd = parse_command("cat < input.txt > output.txt &");
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(cmd.args[0], "cat"); // Command name at args[0]
    EXPECT_EQ(cmd.input_file, "input.txt");
    EXPECT_EQ(cmd.output_file, "output.txt");
    EXPECT_FALSE(cmd.append);
    EXPECT_TRUE(cmd.background);
}

// Test quoted strings in arguments
TEST(ParseCommandTest, QuotedArguments) {
    Command cmd = parse_command("echo \"hello world\"");
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "echo"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "hello world");
}

// Test multiple quotes
TEST(ParseCommandTest, MultipleQuotedArgs) {
    Command cmd = parse_command("echo \"first\" \"second\" \"third\"");
    EXPECT_EQ(cmd.args.size(), 4);
    EXPECT_EQ(cmd.args[0], "echo"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "first");
    EXPECT_EQ(cmd.args[2], "second");
    EXPECT_EQ(cmd.args[3], "third");
}

// Test comment handling (everything after # should be ignored)
TEST(ParseCommandTest, CommentHandling) {
    Command cmd = parse_command("ls -la # this is a comment");
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "ls"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "-la");
}

// Test redirection with quoted filename
TEST(ParseCommandTest, QuotedRedirectedFile) {
    Command cmd = parse_command("echo hello > \"my output.txt\"");
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "echo"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "hello");
    EXPECT_EQ(cmd.output_file, "my output.txt");
    EXPECT_FALSE(cmd.append);
}

// Test append redirection with quoted filename
TEST(ParseCommandTest, AppendQuotedFile) {
    Command cmd = parse_command("echo test >> \"log file.txt\"");
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "echo"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "test");
    EXPECT_EQ(cmd.output_file, "log file.txt");
    EXPECT_TRUE(cmd.append);
}

// Test input redirection with quoted filename
TEST(ParseCommandTest, InputQuotedFile) {
    Command cmd = parse_command("cat < \"input data.txt\"");
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(cmd.args[0], "cat"); // Command name at args[0]
    EXPECT_EQ(cmd.input_file, "input data.txt");
}

// Test no filename after redirection operator (edge case)
TEST(ParseCommandTest, RedirectionWithoutFilename) {
    Command cmd = parse_command("ls >");
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(cmd.args[0], "ls"); // Command name at args[0]
    EXPECT_TRUE(cmd.output_file.empty()); // Should be empty if no filename provided
}

// Test special characters in quotes preserved as arguments
TEST(ParseCommandTest, SpecialCharsInQuotes) {
    Command cmd = parse_command("echo \"test>file\"");
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "echo"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "test>file"); // > should not be treated as redirection inside quotes
}

// Test trailing spaces
TEST(ParseCommandTest, TrailingSpaces) {
    Command cmd = parse_command("ls   ");
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(cmd.args[0], "ls"); // Command name at args[0]
}

// Test leading spaces
TEST(ParseCommandTest, LeadingSpaces) {
    Command cmd = parse_command("  ls -la");
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "ls"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "-la");
}

// Test multiple spaces between tokens
TEST(ParseCommandTest, MultipleSpaces) {
    Command cmd = parse_command("ls   -la   /home");
    EXPECT_EQ(cmd.args.size(), 3);
    EXPECT_EQ(cmd.args[0], "ls"); // Command name at args[0]
    EXPECT_EQ(cmd.args[1], "-la");
    EXPECT_EQ(cmd.args[2], "/home");
}

// Test pipe character (& is handled but | should be too if supported)
TEST(ParseCommandTest, PipeCharacter) {
    Command cmd = parse_command("ls | grep test");
    // Pipes are not supported in single command parsing - only first command is parsed
    EXPECT_EQ(cmd.args.size(), 4);
    EXPECT_EQ(cmd.args[0], "ls");
    EXPECT_TRUE(cmd.input_file.empty());
    EXPECT_TRUE(cmd.output_file.empty());
    EXPECT_FALSE(cmd.append);
    EXPECT_FALSE(cmd.background);
}
