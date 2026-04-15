#!/usr/bin/env bash
# Integration tests for my_shell — pipes and file redirection.
# Each test sends a single command to the shell and checks the output.
# Usage: ./test_integration.sh <path-to-my_shell-binary>

SHELL_BIN="${1:-./build/my_shell}"
PASS=0
FAIL=0
TMP=$(mktemp -d)

run() {
    # run <description> <input-to-shell> <expected-output>
    local desc="$1"
    local input="$2"
    local expected="$3"
    local actual
    # The shell writes its prompt to stdout rather than stderr, and command
    # output appears on the same line as the prompt.  Strip each prompt prefix
    # ("my_shell>...$ ") so only the actual command output remains.
    actual=$(printf '%s\n' "$input" | "$SHELL_BIN" 2>/dev/null \
        | sed 's/my_shell>[^$]*\$ //g' | tr -d '\r' \
        | grep -v '^$' | head -n -1)
    if [ "$actual" = "$expected" ]; then
        echo "PASS: $desc"
        ((PASS++))
    else
        echo "FAIL: $desc"
        echo "      expected: $(printf '%q' "$expected")"
        echo "      actual:   $(printf '%q' "$actual")"
        ((FAIL++))
    fi
}

# ─── Basic commands ───────────────────────────────────────────────────────────

run "echo simple output" \
    "echo hello" \
    "hello"

run "echo multiple args" \
    "echo foo bar baz" \
    "foo bar baz"

# ─── Pipes ────────────────────────────────────────────────────────────────────

run "single pipe" \
    "echo hello | cat" \
    "hello"

run "pipe to grep — matching" \
    "echo foobar | grep foo" \
    "foobar"

run "pipe to grep — no match produces no output" \
    "echo hello | grep xyz" \
    ""

run "two pipes" \
    "echo hello world | tr ' ' '\n' | sort" \
    "$(printf 'hello\nworld')"

run "pipe with wc -w" \
    "echo one two three | wc -w" \
    "3"

# ─── Output redirection ───────────────────────────────────────────────────────

OUT="$TMP/out.txt"

run "redirect output to file" \
    "echo hello > $OUT" \
    ""   # shell produces no stdout; file is checked below

if [ "$(cat "$OUT" 2>/dev/null | tr -d '\r')" = "hello" ]; then
    echo "PASS: redirected content written to file"
    ((PASS++))
else
    echo "FAIL: redirected content written to file"
    echo "      file contained: $(cat "$OUT" 2>/dev/null)"
    ((FAIL++))
fi

run "append redirect adds to file" \
    "echo world >> $OUT" \
    ""

CONTENT=$(cat "$OUT" | tr -d '\r')
EXPECTED=$(printf 'hello\nworld')
if [ "$CONTENT" = "$EXPECTED" ]; then
    echo "PASS: append redirect appended to existing file"
    ((PASS++))
else
    echo "FAIL: append redirect appended to existing file"
    echo "      expected: $(printf '%q' "$EXPECTED")"
    echo "      actual:   $(printf '%q' "$CONTENT")"
    ((FAIL++))
fi

# ─── Input redirection ────────────────────────────────────────────────────────

IN="$TMP/in.txt"
printf 'banana\napple\ncherry\n' > "$IN"

run "redirect input from file" \
    "cat < $IN" \
    "$(printf 'banana\napple\ncherry')"

run "sort with input redirection" \
    "sort < $IN" \
    "$(printf 'apple\nbanana\ncherry')"

# ─── Pipe + redirection combined ─────────────────────────────────────────────

COMBINED="$TMP/combined.txt"

run "pipe output into file via redirect" \
    "echo hello | cat > $COMBINED" \
    ""

if [ "$(cat "$COMBINED" 2>/dev/null | tr -d '\r')" = "hello" ]; then
    echo "PASS: pipe result redirected to file"
    ((PASS++))
else
    echo "FAIL: pipe result redirected to file"
    echo "      file contained: $(cat "$COMBINED" 2>/dev/null)"
    ((FAIL++))
fi

run "input file piped through grep" \
    "grep apple < $IN" \
    "apple"

# ─── Cleanup & summary ────────────────────────────────────────────────────────

rm -rf "$TMP"
echo ""
echo "Results: $PASS passed, $FAIL failed"
[ "$FAIL" -eq 0 ]
