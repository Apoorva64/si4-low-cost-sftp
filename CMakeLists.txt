cmake_minimum_required(VERSION 3.22)
project(SecTrans)


enable_testing()
set(JANSSON_BUILD_SHARED_LIBS ON)
set(JANSSON_BUILD_DOCS OFF)
set(CPP_JWT_BUILD_TESTS OFF)
add_subdirectory(external-libs/libclient)
add_subdirectory(external-libs/libserver)
add_subdirectory(external-libs/OpenSSL)
add_subdirectory(external-libs/CLI11)
add_subdirectory(external-libs/spdlog)
add_subdirectory(external-libs/jwt-cpp)
add_subdirectory(external-libs/curlcpp)
add_subdirectory(external-libs/ThreadPool)
add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(peer-server)
add_subdirectory(client)
add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 20)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)