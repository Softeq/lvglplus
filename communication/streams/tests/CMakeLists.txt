cmake_minimum_required(VERSION 3.14)
project(stream_tests
        VERSION 0.0.1
        DESCRIPTION "LVGL plus tests"
        LANGUAGES C CXX)

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_executable(
        stream_tests
        stream_test.cc
        main.cc
)

target_link_libraries(stream_tests GTest::GTest streams thread_blocker)

add_test(stream_tests stream_tests)