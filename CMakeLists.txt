cmake_minimum_required(VERSION 3.22)
project(SecTrans)
add_subdirectory(external-libs/libclient)
add_subdirectory(external-libs/libserver)
add_subdirectory(external-libs/AES)
add_subdirectory(external-libs/OpenSSL)
add_subdirectory(external-libs/CLI11)
add_subdirectory(common)

set(CMAKE_CXX_STANDARD 23)
find_package(OpenSSL REQUIRED)
add_executable(SecTransServer src/server/main.cpp
        src/server/server/Server.cpp
        src/server/server/Server.h
)
target_link_libraries(SecTransServer libclient libserver AES OpenSSL common)
add_executable(SecTransClient src/client/main.cpp
        src/client/client/Client.cpp
        src/client/client/Client.h)
target_link_libraries(SecTransClient libclient libserver AES OpenSSL CLI11 common)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#
#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)