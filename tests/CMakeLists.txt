cmake_minimum_required(VERSION 3.14)
project(lvgl_plus_tests
        VERSION 0.0.1
        DESCRIPTION "LVGL plus tests"
        LANGUAGES C CXX)

find_package(GTest REQUIRED)

add_executable(
        test_widget
        test_widget.cc
        main.cc
)

add_executable(
        test_widget_params
        test_widget_params.cc
        main.cc
)

add_executable(
        test_lvgl_plus
        test_lvgl_plus.cc
        main.cc
)

add_executable(
        test_bitmap
        test_bitmap.cc
        main.cc
)

SET(lv_libs SDL2 lvgl_plus::lvgl_plus lvgl::drivers lvgl::lvgl GTest::GTest)

target_link_libraries(test_widget ${lv_libs})
target_link_libraries(test_widget_params ${lv_libs})
target_link_libraries(test_lvgl_plus ${lv_libs})
target_link_libraries(test_bitmap ${lv_libs})

add_test(test_widget test_widget)
add_test(test_widget_params test_widget_params)
add_test(test_lvgl_plus test_lvgl_plus)
add_test(test_bitmap test_bitmap)