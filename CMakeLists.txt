cmake_minimum_required(VERSION 3.8)

project(keepit)

# Setup build type
set(CMAKE_BUILD_TYPE "Release")

# Setup C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Setup include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/core)

# Add executable
add_executable(unique src/main.cpp
    src/StreamDispatcher.cpp
    src/WordsCollection.cpp
    src/WordsCounter.cpp

    src/core/ExecutionFactory.cpp
    src/core/ExecutionPool.cpp
    src/core/TaskProviderList.cpp
    src/core/ThreadWorker.cpp
    src/core/CancelTokenProvider.cpp)
