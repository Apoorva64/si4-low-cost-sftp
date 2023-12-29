cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
add_executable(SecTransServer main.cpp
        server/Server.cpp
        server/Server.h
        server/PeerServer.cpp
        server/PeerServer.h
        server/SessionStorage.cpp
        server/SessionStorage.h
)
#INCLUDE_DIRECTORIES(${CURLCPP_SOURCE_DIR}/include)
target_link_libraries(SecTransServer libclient libserver ThreadPool OpenSSL common spdlog curlcpp nlohmann_json::nlohmann_json jwt-cpp)

# build release
#set(CMAKE_BUILD_TYPE Release)
