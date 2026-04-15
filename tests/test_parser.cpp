#include <gtest/gtest.h>
#include "parser.h"
#include "pipeline.h"

// ─── tokenize ────────────────────────────────────────────────────────────────

TEST(Tokenize, SingleWord) {
    auto tokens = shell::tokenize("ls");
    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0], "ls");
}

TEST(Tokenize, MultipleWords) {
    auto tokens = shell::tokenize("ls -la /tmp");
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], "-la");
    EXPECT_EQ(tokens[2], "/tmp");
}

TEST(Tokenize, LeadingAndTrailingSpaces) {
    auto tokens = shell::tokenize("  echo hello  ");
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "hello");
}

TEST(Tokenize, QuotedStringPreservesSpaces) {
    auto tokens = shell::tokenize("echo \"hello world\"");
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "hello world");
}

TEST(Tokenize, PipeIsItsOwnToken) {
    auto tokens = shell::tokenize("ls | grep foo");
    ASSERT_EQ(tokens.size(), 4u);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], "|");
    EXPECT_EQ(tokens[2], "grep");
    EXPECT_EQ(tokens[3], "foo");
}

TEST(Tokenize, RedirectOutIsItsOwnToken) {
    auto tokens = shell::tokenize("echo hi > out.txt");
    ASSERT_EQ(tokens.size(), 4u);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "hi");
    EXPECT_EQ(tokens[2], ">");
    EXPECT_EQ(tokens[3], "out.txt");
}

TEST(Tokenize, AppendRedirectIsDoubleChevron) {
    auto tokens = shell::tokenize("echo hi >> out.txt");
    ASSERT_EQ(tokens.size(), 4u);
    EXPECT_EQ(tokens[2], ">>");
    EXPECT_EQ(tokens[3], "out.txt");
}

TEST(Tokenize, RedirectInIsItsOwnToken) {
    auto tokens = shell::tokenize("sort < input.txt");
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[1], "<");
}

TEST(Tokenize, CommentIsIgnored) {
    auto tokens = shell::tokenize("echo hello # this is a comment");
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0], "echo");
    EXPECT_EQ(tokens[1], "hello");
}

TEST(Tokenize, BackgroundAmpersandIsToken) {
    auto tokens = shell::tokenize("sleep 1 &");
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[2], "&");
}

TEST(Tokenize, EmptyLineReturnsNoTokens) {
    auto tokens = shell::tokenize("");
    EXPECT_TRUE(tokens.empty());
}

// ─── parse_command ───────────────────────────────────────────────────────────

TEST(ParseCommand, SingleCommand) {
    auto pip = shell::parse_command("ls -la");
    ASSERT_EQ(pip.commands.size(), 1u);
    EXPECT_EQ(pip.commands[0].name, "ls");
    EXPECT_EQ(pip.commands[0].args, (std::vector<std::string>{"ls", "-la"}));
}

TEST(ParseCommand, TwoCommandPipeline) {
    auto pip = shell::parse_command("ls | grep foo");
    ASSERT_EQ(pip.commands.size(), 2u);
    EXPECT_EQ(pip.commands[0].name, "ls");
    EXPECT_EQ(pip.commands[1].name, "grep");
    EXPECT_EQ(pip.commands[1].args, (std::vector<std::string>{"grep", "foo"}));
}

TEST(ParseCommand, ThreeCommandPipeline) {
    auto pip = shell::parse_command("cat file.txt | grep error | wc -l");
    ASSERT_EQ(pip.commands.size(), 3u);
    EXPECT_EQ(pip.commands[0].name, "cat");
    EXPECT_EQ(pip.commands[1].name, "grep");
    EXPECT_EQ(pip.commands[2].name, "wc");
}

TEST(ParseCommand, OutputRedirection) {
    auto pip = shell::parse_command("echo hello > out.txt");
    ASSERT_EQ(pip.commands.size(), 1u);
    EXPECT_EQ(pip.output_file, "out.txt");
    EXPECT_FALSE(pip.append);
}

TEST(ParseCommand, AppendRedirection) {
    auto pip = shell::parse_command("echo hello >> out.txt");
    ASSERT_EQ(pip.commands.size(), 1u);
    EXPECT_EQ(pip.output_file, "out.txt");
    EXPECT_TRUE(pip.append);
}

TEST(ParseCommand, InputRedirection) {
    auto pip = shell::parse_command("sort < input.txt");
    ASSERT_EQ(pip.commands.size(), 1u);
    EXPECT_EQ(pip.input_file, "input.txt");
}

TEST(ParseCommand, InputAndOutputRedirection) {
    auto pip = shell::parse_command("sort < input.txt > output.txt");
    EXPECT_EQ(pip.input_file, "input.txt");
    EXPECT_EQ(pip.output_file, "output.txt");
}

TEST(ParseCommand, PipeWithOutputRedirection) {
    auto pip = shell::parse_command("ls | grep foo > result.txt");
    ASSERT_EQ(pip.commands.size(), 2u);
    EXPECT_EQ(pip.output_file, "result.txt");
}

TEST(ParseCommand, BackgroundFlag) {
    auto pip = shell::parse_command("sleep 5 &");
    EXPECT_TRUE(pip.background);
}

TEST(ParseCommand, EmptyLineReturnsEmptyPipeline) {
    auto pip = shell::parse_command("");
    EXPECT_TRUE(pip.commands.empty());
    EXPECT_TRUE(pip.input_file.empty());
    EXPECT_TRUE(pip.output_file.empty());
}

TEST(ParseCommand, CommandArgsArePreserved) {
    auto pip = shell::parse_command("grep -r --include=\"*.cpp\" pattern");
    ASSERT_EQ(pip.commands.size(), 1u);
    EXPECT_EQ(pip.commands[0].args[1], "-r");
    EXPECT_EQ(pip.commands[0].args[2], "--include=*.cpp");
    EXPECT_EQ(pip.commands[0].args[3], "pattern");
}
