mkdir build local_files
touch local_files/users.txt
clang++ -Wall -Wextra -Wpedantic -Werror -std=c++23 main.cpp -o ./build/main
echo "Result is in ./build folder"