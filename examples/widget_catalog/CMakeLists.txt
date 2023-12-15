cmake_minimum_required(VERSION 3.14)
project(widget_catalog
        VERSION 0.0.1
        DESCRIPTION "Widget catalog"
        LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(widget_catalog
        main.cc
        )

set(lv_libs SDL2 lvgl_plus::lvgl_plus lvgl::drivers lvgl::lvgl)
target_link_libraries(widget_catalog ${lv_libs})
