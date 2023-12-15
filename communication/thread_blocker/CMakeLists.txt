cmake_minimum_required(VERSION 3.14)
project(thread_blocker
        VERSION 0.0.1
        DESCRIPTION "Thread blocker"
        LANGUAGES C CXX)

set(thread_blocker_sources
        src/thread_blocker.cc
        )

add_library(thread_blocker STATIC
        ${thread_blocker_sources}
        )
target_include_directories(thread_blocker PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (BUILD_TESTING)
  add_subdirectory(tests)
endif ()
