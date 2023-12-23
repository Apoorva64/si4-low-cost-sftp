cmake_minimum_required(VERSION 3.22)
project(SecTrans)
add_subdirectory(libclient)
add_subdirectory(libserver)

set(CMAKE_CXX_STANDARD 23)
find_package(OpenSSL REQUIRED)
add_executable(SecTransServer src/server/main.cpp
        src/common/SocketCommunication/SocketCommunication.cpp
        src/common/SocketCommunication/SocketCommunication.h
        src/common/AES/AES.cpp
        src/common/AES/AES.h
        src/common/OpenSSL/OpenSSL.cpp
        src/common/OpenSSL/OpenSSL.h
        src/server/server/Server.cpp
        src/server/server/Server.h
)
target_link_libraries(SecTransServer OpenSSL::SSL libclient libserver)
add_executable(SecTransClient src/client/main.cpp
        src/common/SocketCommunication/SocketCommunication.cpp
        src/common/SocketCommunication/SocketCommunication.h
        src/common/AES/AES.cpp
        src/common/AES/AES.h
        src/common/OpenSSL/OpenSSL.cpp
        src/common/OpenSSL/OpenSSL.h
        src/client/client/Client.cpp
        src/client/client/Client.h)
target_link_libraries(SecTransClient OpenSSL::SSL libclient libserver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)