### To successfuly build and use please refer to setup of cppfront/cpp2 in root of the repository and this file

- ```mkdir build local_files```
- ```touch local_users/users.txt```
- ```cppfront main.cpp2 -p```
- ```clang++ main.cpp -std=c++20 -I<YOUR_CPPFRONT_INCLUDE_PATH> -o ./build/main```

Remark about warnings compiler shows - they are related to how cppfront translates cpp2 to cpp. (double parentheses)