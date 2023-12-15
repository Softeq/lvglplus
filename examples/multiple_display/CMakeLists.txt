cmake_minimum_required(VERSION 3.14)
project(multiple_display
        VERSION 0.0.1
        DESCRIPTION "Widget catalog"
        LANGUAGES C CXX)

add_executable(multiple_display_example
        main.cc
        )

set(lv_libs SDL2 lvgl_plus::lvgl_plus lvgl::drivers lvgl::lvgl)
target_link_libraries(multiple_display_example ${lv_libs})
