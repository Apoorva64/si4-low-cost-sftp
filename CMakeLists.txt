cmake_minimum_required(VERSION 3.22)
project(SecTrans)

set(CMAKE_CXX_STANDARD 23)

add_executable(SecTransServer src/server/main.cpp
        src/common/SocketCommunication/SocketCommunication.cpp
        src/common/SocketCommunication/SocketCommunication.h)
target_link_libraries(SecTransServer ${PROJECT_SOURCE_DIR}/lib/libserver.so ${PROJECT_SOURCE_DIR}/lib/libclient.so)
add_executable(SecTransClient src/client/main.cpp
        src/common/SocketCommunication/SocketCommunication.cpp
        src/common/SocketCommunication/SocketCommunication.h
        src/client/client/client.cpp
        src/client/client/client.h)
target_link_libraries(SecTransClient ${PROJECT_SOURCE_DIR}/lib/libclient.so ${PROJECT_SOURCE_DIR}/lib/libserver.so)
