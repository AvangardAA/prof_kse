#!/bin/bash

tests=(
    "dummy"
    "dummy"
    "dummy2"
    "dummy delete"
    "bread delete"
    ""
    "nan delete"
)

run_test() {
    local input="$1"
    local expected_output="$2"
    local test_name="$3"

    output=$(echo "" | ./build/main $input)

    if [ "$output" = "$expected_output" ]; then
        echo -e "Test '${test_name}' \033[92mPASSED ✓\033[0m"
    else
        echo -e "Test '${test_name}' \033[91mFAILED ✗\033[0m"
        echo "Expected: ${expected_output}"
        echo "Got:      ${output}"
    fi
}

if [ ! -f "./build/main" ]; then
    echo "Compile first, then test."
    exit 1
fi

run_test "${tests[0]}" "Welcome, dummy!" "$((1)) usual login"
run_test "${tests[1]}" "Hello again(x2), dummy" "$((2)) next login, same user"
run_test "${tests[2]}" "Welcome, dummy2!" "$((3)) login other user"
run_test "${tests[3]}" "Successfully deleted user." "$((4)) delete dummy user"
run_test "${tests[0]}" "Welcome, dummy!" "$((5)) verify user was deleted"
run_test "${tests[4]}" "Successfully deleted all users." "$((6)) delete all users"
run_test "${tests[0]}" "Welcome, dummy!" "$((7)) verify dummy user after delete"
run_test "${tests[2]}" "Welcome, dummy2!" "$((8)) verify dummy2 user after delete"
run_test "${tests[4]}" "Successfully deleted all users." "$((9)) delete all users again"
run_test "${tests[5]}" "Unsupported number of args" "$((10)) empty input"
run_test "${tests[6]}" "Nothing to delete, user not found." "$((11)) user isnt in records"