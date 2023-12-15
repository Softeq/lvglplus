cmake_minimum_required(VERSION 3.14)
project(communication
        VERSION 0.0.1
        DESCRIPTION "Dart-like stream"
        LANGUAGES C CXX)

add_subdirectory(thread_blocker)
add_subdirectory(streams)
