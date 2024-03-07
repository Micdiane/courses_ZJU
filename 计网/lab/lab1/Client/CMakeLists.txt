cmake_minimum_required(VERSION 3.25)
project(Client)

set(CMAKE_CXX_STANDARD 14)

add_executable(Client
        Lib.h
        client.cpp)

# 指定链接的库，例如 ws2_32
target_link_libraries(Client ws2_32)
