cmake_minimum_required(VERSION 3.8)

project(client)

# Setting the C++ standard directly
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Add the executable
add_executable(client main.cpp)
