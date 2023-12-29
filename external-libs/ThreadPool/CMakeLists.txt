# Declare the library
add_library(ThreadPool STATIC
        ThreadPool.cpp
        ThreadPool.h
)


set_target_properties(ThreadPool PROPERTIES LINKER_LANGUAGE CXX)

# Specify here the include directories exported
# by this library
target_include_directories(ThreadPool PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)