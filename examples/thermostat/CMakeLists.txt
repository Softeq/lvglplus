cmake_minimum_required(VERSION 3.14)
project(thermostat_example_project
        VERSION 0.0.1
        DESCRIPTION "Thermostat example"
        LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(thermostat_sources
        thermostat.cc
        main_page.cc
        settings_page.cc
        )

add_executable(thermostat_example
        main.cc
        ${thermostat_sources}
        )

set(lv_libs SDL2 lvgl_plus::lvgl_plus lvgl::drivers lvgl::lvgl)
target_link_libraries(thermostat_example ${lv_libs})
