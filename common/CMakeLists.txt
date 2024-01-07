

# Declare the library
add_library(common STATIC
        SocketCommunication/SocketCommunication.cpp
        SocketCommunication/SocketCommunication.h
        Command/CommandEnum.h
        Command/Command.cpp
        Command/Command.h
        Response/Response.cpp
        Response/Response.h
        Args/Arguments.cpp
        Args/Arguments.h
        Response/Response.h
        Response/ResponseEnum.h
)


set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(common libclient libserver OpenSSL spdlog)

# Specify here the include directories exported
# by this library
target_include_directories(common PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
