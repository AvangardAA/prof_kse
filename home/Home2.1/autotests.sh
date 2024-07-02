#!/bin/bash

tests=(
    "dummy dummy"
    "dummy dummy dummy"
    ""
    "dummy"
)

run_test() {
    local input="$1"
    local expected_substring="$2"
    local test_name="$3"

    output=$(echo "" | ./build/main $input)

    if echo "$output" | grep -q "$expected_substring"; then
        echo -e "Test '${test_name}' \033[92mPASSED ✓\033[0m"
    else
        echo -e "Test '${test_name}' \033[91mFAILED ✗\033[0m"
        echo "Expected substring: ${expected_substring}"
        echo "Output: ${output}"
    fi
}


if [ ! -f "./build/main" ]; then
    echo "Compile first, then test."
    exit 1
fi

run_test "${tests[0]}" "Unsupported second argument. Support only: 'delete'." "$((1)) Wrong second arg."
run_test "${tests[1]}" "Unsupported number of args" "$((2)) Too many args."
run_test "${tests[2]}" "Unsupported number of args" "$((3)) Not enough args."
run_test "${tests[3]}" "dummy" "$((4)) Login any test."
run_test "${tests[3]}" "Hello" "$((5)) Consecutive login, only hello again answer accepted."