# Set the minimum required version of CMake.
# 3.10 is a good baseline for modern C++.
cmake_minimum_required(VERSION 3.10)

# Set the project name. This will be the name of your executable.
project(MyGameProject CXX)

# Specify the C++ standard to use.
# Your code uses C++11/17 features, so this is important.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all your source files (.cpp) automatically.
# This is convenient if you add more .cpp files later.
# It assumes all your .cpp files are in the same directory as this CMakeLists.txt.
file(GLOB SOURCES "*.cpp")

# Add the executable target.
# It will be named "MyGameProject" (from the project() command).
# It will be built from all the source files found by the file(GLOB) command.
add_executable(${PROJECT_NAME} ${SOURCES})

# If you prefer to list your source files manually, you can do this instead:
# add_executable(MyGameProject main.cpp player.cpp board.cpp)
# Note: You only list the .cpp files, not the .h files.