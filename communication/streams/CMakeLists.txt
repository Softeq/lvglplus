cmake_minimum_required(VERSION 3.14)
project(streams
        VERSION 0.0.1
        DESCRIPTION "Dart-like stream"
        LANGUAGES C CXX)

set(stream_sources
        src/stream_event_loop.cc
        )

add_library(streams STATIC
        ${stream_sources}
        )

target_include_directories(streams PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(streams thread_blocker)

if (BUILD_TESTING)
  add_subdirectory(tests)
endif ()
