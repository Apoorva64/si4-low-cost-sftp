cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
add_executable(SecTransServer main.cpp
        server/Server.cpp
        server/Server.h
)
#INCLUDE_DIRECTORIES(${CURLCPP_SOURCE_DIR}/include)
target_link_libraries(SecTransServer libclient libserver AES OpenSSL common spdlog curlcpp nlohmann_json::nlohmann_json jwt-cpp)