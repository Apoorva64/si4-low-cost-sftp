cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
add_executable(SecTransServer main.cpp
        server/Server.cpp
        server/Server.h
)
#INCLUDE_DIRECTORIES(${CURLCPP_SOURCE_DIR}/include)
target_link_libraries(SecTransServer libclient libserver CLI11 ThreadPool OpenSSL common spdlog curlcpp nlohmann_json::nlohmann_json jwt-cpp)
# copy a the built executable to the peer-server directory every time it is built
add_custom_command(TARGET SecTransServer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/SecTransServer
        ${CMAKE_SOURCE_DIR}/peer-server/SecTransServer
        COMMENT "Copying SecTransServer to peer-server directory" VERBATIM
        )
# add Wall flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# build release
#set(CMAKE_BUILD_TYPE Release)
