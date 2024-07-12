#!/bin/bash

file_path=(
    "image1.txt"
    "image2.txt"
    "image3.txt"
    "image4.txt"
    "image5.txt"
    "image6.txt"
)

if [ ! -f "./build/main" ]; then
    echo "Compile first, then test."
    exit 1
fi

run_test() {
    local input_file="$1"
    local next_str="$2"
    local output_file="$3"
    local expected_output_file="$4"

    if [[ $# -eq 6 ]]; then
        local unfav_color="$5"
        local test_name="$6"
        ./build/main "$input_file" "$next_str" "$output_file" "$unfav_color"
    else
        local test_name="$5"
        ./build/main "$input_file" "$next_str" "$output_file"
    fi

    if cmp -s "local_files/$output_file" "$expected_output_file"; then
        echo -e "Test '${test_name}' \033[92mPASSED ✓\033[0m"
    else
        echo -e "Test '${test_name}' \033[91mFAILED ✗\033[0m"
        echo "Expected output file: ${expected_output_file}"
        echo "Actual output file:"
        cat "local_files/$output_file"
    fi
}

run_test "${file_path[0]}" "red" "out1.txt" "local_files/tests/out1_verify.txt" "1 Left column and third red"
run_test "${file_path[1]}" "red" "out2.txt" "local_files/tests/out2_verify.txt" "2 Red hammer inside test"
run_test "${file_path[2]}" "blue" "out3.txt" "local_files/tests/out3_verify.txt" "3 Blue left corner test"
run_test "${file_path[3]}" "green" "out4.txt" "local_files/tests/out4_verify.txt" "4 Green wall inside test"
run_test "${file_path[4]}" "green" "out5.txt" "local_files/tests/out5_verify.txt" "5 Green inside image"
run_test "${file_path[5]}" "blue" "out6.txt" "local_files/tests/out6_verify.txt" "red" "6 Change all red to blue"