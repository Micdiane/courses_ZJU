cmake_minimum_required(VERSION 3.25)
project(Server)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server
        Lib.h
        server.cpp)

# 指定链接的库，例如 ws2_32
target_link_libraries(Server ws2_32)
