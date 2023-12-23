

# Declare the library
add_library(libserver STATIC
        libserver.h
)

target_link_libraries(libserver  ${CMAKE_CURRENT_SOURCE_DIR}/libserver.so
)
set_target_properties(libserver PROPERTIES LINKER_LANGUAGE CXX)

# Specify here the include directories exported
# by this library
target_include_directories(libserver PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
