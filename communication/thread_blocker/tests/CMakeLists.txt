cmake_minimum_required(VERSION 3.14)
project(thread_blocker_tests
        VERSION 0.0.1
        DESCRIPTION "Thread blocker tests"
        LANGUAGES C CXX)

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_executable(
        thread_blocker_tests
        thread_blocker_test.cc
        main.cc
)

target_link_libraries(thread_blocker_tests GTest::GTest thread_blocker)

add_test(thread_blocker_tests thread_blocker_tests)