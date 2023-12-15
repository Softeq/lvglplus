cmake_minimum_required(VERSION 3.14)
project(heater_example_project
        VERSION 0.0.1
        DESCRIPTION "Heater example"
        LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(heater_sources
        main_page.cc
        settings_page.cc
        heater.cc
        )

add_executable(heater_example
        main.cc
        ${heater_sources}
        )

set(lv_libs SDL2 lvgl_plus::lvgl_plus lvgl::drivers lvgl::lvgl)
target_link_libraries(heater_example ${lv_libs})

if (BUILD_GOLDEN_TESTS)
    set(ACCEPTABLE_MISMATCH_PERCENTAGE "0" CACHE STRING "")
    # an acceptable mismatch percentage of pixels between the source and a real image of the display
    add_definitions(-DACCEPTABLE_MISMATCH_PERCENTAGE=${ACCEPTABLE_MISMATCH_PERCENTAGE})

    if (UPDATE_GOLDEN_SAMPLES)
        add_definitions(-DMAKE_GOLDEN_TESTS_SAMPLES)
    endif ()

    # paths to images
    set(GOLDEN_SOURCE_DIR ${PROJECT_SOURCE_DIR}/golden_tests)
    set(GOLDEN_SAMPLES_SOURCE_DIR ${GOLDEN_SOURCE_DIR}/samples)
    set(GOLDEN_BINARY_DIR ${PROJECT_BINARY_DIR}/golden_tests)
    set(GOLDEN_SAMPLES_BINARY_DIR ${GOLDEN_BINARY_DIR}/samples)
    set(GOLDEN_FAILED_BINARY_DIR ${GOLDEN_BINARY_DIR}/failed)
    add_definitions(-DGOLDEN_FAILED_BINARY_DIR="${GOLDEN_FAILED_BINARY_DIR}")
    add_definitions(-DGOLDEN_SAMPLES_BINARY_DIR="${GOLDEN_SAMPLES_BINARY_DIR}")

    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${GOLDEN_SAMPLES_SOURCE_DIR}
            ${GOLDEN_SAMPLES_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${GOLDEN_FAILED_BINARY_DIR})

    # Google tests
    find_package(GTest REQUIRED)
    include(GoogleTest)
    enable_testing()

    add_executable(
            test_heater_golden
            golden_tests/test_golden.cc
            golden_tests/main.cc
            ${heater_sources}
    )

    target_include_directories(test_heater_golden PUBLIC golden_tests)
    target_link_libraries(test_heater_golden PUBLIC ${lv_libs} GTest::GTest)
endif ()
