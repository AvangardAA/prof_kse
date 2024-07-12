mkdir build local_files
touch local_files/inventory.txt
clang++ -Wall -Wextra -Wpedantic -Werror -std=c++23 -Iinclude src/test.cpp src/Ingredient.cpp src/Menu.cpp src/Builder.cpp -o ./build/test
echo "Result is in ./build folder"