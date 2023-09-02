cmake_minimum_required(VERSION 3.22)
project(scdd)

# set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_COMPILER             "clang++")
set(CMAKE_CXX_FLAGS                "-Wall -std=c++2b")
set(CMAKE_CXX_FLAGS_DEBUG          "-gfull")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

include_directories(include)
file(GLOB_RECURSE _sources
    "include/*.hpp"
    "include/**/*.hpp"
    "src/*.cpp"
    "src/**/*.cpp"
)

add_executable(${PROJECT_NAME} ${_sources})