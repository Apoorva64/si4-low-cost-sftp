

# Declare the library
add_library(libclient STATIC
        libclient.h
)

target_link_libraries(libclient  ${CMAKE_CURRENT_SOURCE_DIR}/libclient.so
)
set_target_properties(libclient PROPERTIES LINKER_LANGUAGE CXX)

# Specify here the include directories exported
# by this library
target_include_directories(libclient PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
