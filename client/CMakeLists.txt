cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

add_executable(SecTransClient main.cpp
        client/Client.cpp
        client/Client.h
)
find_library(curl NAMES curl)
#include(FetchContent)
#
#FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
#FetchContent_MakeAvailable(json)

target_link_libraries(SecTransClient libclient libserver OpenSSL CLI11 common jwt-cpp)

#set(CMAKE_BUILD_TYPE Release)
