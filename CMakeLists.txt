# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Name of your project
project(TicTacToeBot CXX)

# Set the C++ standard we want to use
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake to create an executable named "tictactoe_bot"
# from the main.cpp source file. CMake will automatically
# figure out the header dependencies.
add_executable(
        generate_csv_executable
        create_csv.cpp
)

# Optional: Add compiler flags for warnings
target_compile_options(generate_csv_executable PRIVATE -Wall -Wextra)