#!/bin/bash

# 编译client.cpp并生成可执行文件client
g++ client/client.cpp -o miniclient

# 编译server.cpp并生成可执行文件server
g++ server/server.cpp -o miniserver

