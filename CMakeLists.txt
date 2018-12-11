cmake_minimum_required(VERSION 3.8)

project(client)

# Setting the C++ standard directly
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the executable
add_executable(client main.cpp wrapsock.h udp_client.h wrapstdio.h)

# Add link libraries
target_link_libraries(client LINK_PRIVATE Ws2_32)
