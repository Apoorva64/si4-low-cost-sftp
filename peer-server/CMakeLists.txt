cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
add_executable(SecTransPeerServer main.cpp
        PeerServer/PeerServer.cpp
        PeerServer/PeerServer.h
        SessionStorage/SessionStorage.cpp
        SessionStorage/SessionStorage.h
)
#INCLUDE_DIRECTORIES(${CURLCPP_SOURCE_DIR}/include)
target_link_libraries(SecTransPeerServer libclient libserver ThreadPool OpenSSL common spdlog curlcpp nlohmann_json::nlohmann_json jwt-cpp)
# static linking
add_dependencies(SecTransPeerServer SecTransServer)
# Copy SecTransServer executable from /server build directory to /build directory of SecTransPeerServer project. Delete old executable first be verbose
add_custom_command(TARGET SecTransPeerServer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove -f SecTransServer
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/server/SecTransServer ${CMAKE_CURRENT_BINARY_DIR}/SecTransServer
        COMMENT "Copying SecTransServer to build directory"
        )
include(GNUInstallDirs)

# build release
set(CMAKE_BUILD_TYPE Release)
