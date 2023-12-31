
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests test_crypt.cpp
        test_thread_pool.cpp
        test_utils.cpp)
target_link_libraries(tests Catch2::Catch2WithMain OpenSSL ThreadPool)
include(CTest)
include(Catch)
catch_discover_tests(tests)
