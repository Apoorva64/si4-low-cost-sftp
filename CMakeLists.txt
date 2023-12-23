cmake_minimum_required(VERSION 3.22)
project(SecTrans)

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
target_link_libraries(SecTransServer ${PROJECT_SOURCE_DIR}/lib/libserver.so ${PROJECT_SOURCE_DIR}/lib/libclient.so OpenSSL::SSL)
add_executable(SecTransClient src/client/main.cpp
        src/common/SocketCommunication/SocketCommunication.cpp
        src/common/SocketCommunication/SocketCommunication.h
        src/common/AES/AES.cpp
        src/common/AES/AES.h
        src/common/OpenSSL/OpenSSL.cpp
        src/common/OpenSSL/OpenSSL.h
        src/client/client/Client.cpp
        src/client/client/Client.h)
target_link_libraries(SecTransClient ${PROJECT_SOURCE_DIR}/lib/libclient.so ${PROJECT_SOURCE_DIR}/lib/libserver.so OpenSSL::SSL)
