

# Declare the library
add_library(catch STATIC
        catch.hpp
)


set_target_properties(catch PROPERTIES LINKER_LANGUAGE CXX)

# Specify here the include directories exported
# by this library
target_include_directories(catch PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
