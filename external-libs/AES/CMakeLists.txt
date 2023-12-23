

# Declare the library
add_library(AES STATIC
        AES.h
        AES.cpp
)


set_target_properties(AES PROPERTIES LINKER_LANGUAGE CXX)

# Specify here the include directories exported
# by this library
target_include_directories(AES PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
