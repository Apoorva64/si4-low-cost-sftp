cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
add_executable(SecTransPeerServer main.cpp
        PeerServer/PeerServer.cpp
        PeerServer/PeerServer.h
        SessionStorage/SessionStorage.cpp
        SessionStorage/SessionStorage.h
)
#INCLUDE_DIRECTORIES(${CURLCPP_SOURCE_DIR}/include)
target_link_libraries(SecTransPeerServer libclient libserver ThreadPool OpenSSL common spdlog curlcpp nlohmann_json::nlohmann_json jwt-cpp)

# Copy SecTransServer executable with POST_BUILD command
add_custom_command(TARGET SecTransPeerServer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/SecTransServer
        ${CMAKE_CURRENT_BINARY_DIR}/SecTransServer)

# build release
#set(CMAKE_BUILD_TYPE Release)
