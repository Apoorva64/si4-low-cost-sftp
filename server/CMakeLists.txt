cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
add_executable(SecTransServer main.cpp
        server/Server.cpp
        server/Server.h
)
target_link_libraries(SecTransServer libclient libserver AES OpenSSL common spdlog)