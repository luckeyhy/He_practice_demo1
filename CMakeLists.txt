cmake_minimum_required(VERSION 3.10)

project(demo)

set(CMAKE_CXX_STANDARD 11)

include_directories(inc)

set(SOURCES
    src/scheduler.cpp
    src/main.cpp
)

add_executable(result ${SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(result Threads::Threads)