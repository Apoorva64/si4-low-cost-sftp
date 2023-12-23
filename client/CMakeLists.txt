cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)

add_executable(SecTransClient main.cpp
        client/Client.cpp
        client/Client.h)
target_link_libraries(SecTransClient libclient libserver AES OpenSSL CLI11 common)