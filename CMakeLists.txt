cmake_minimum_required(VERSION 3.8)

project(keepit)

# Setup build type
set(CMAKE_BUILD_TYPE "Release")

# Setup C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/execq)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/execq)

# Add executable
add_executable(client client.cpp)
add_executable(server server.cpp)
add_executable(toilet toilet.cpp WordsCounter.cpp
    src/execq/execq.cpp
    src/execq/ExecutionPool.cpp
    src/execq/ExecutionStream.cpp
    src/execq/TaskProviderList.cpp
    src/execq/ThreadWorker.cpp
    src/execq/CancelTokenProvider.cpp
)
