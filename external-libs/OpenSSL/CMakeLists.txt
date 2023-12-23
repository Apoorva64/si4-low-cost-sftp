
find_package(OpenSSL REQUIRED)
# Declare the library
add_library(OpenSSL STATIC
        OpenSSL.cpp
        OpenSSL.h
        OpenSSL_AES.cpp
        OpenSSL_Utils.cpp
        OpenSSL_Utils.h
)

target_link_libraries(OpenSSL
        OpenSSL::SSL
        OpenSSL::Crypto
        )


set_target_properties(OpenSSL PROPERTIES LINKER_LANGUAGE CXX)

# Specify here the include directories exported
# by this library
target_include_directories(OpenSSL PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
