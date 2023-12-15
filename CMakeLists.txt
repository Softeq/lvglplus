cmake_minimum_required(VERSION 3.14)
project(lvgl_plus
        VERSION 0.0.1
        DESCRIPTION "C++ lvgl wrapper"
        LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wall -O0)

include(cmake/utils.cmake)

add_library(lvgl_plus) # initialized below
add_library(lvgl_plus::lvgl_plus ALIAS lvgl_plus)
find_package(SDL2 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR})
add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
target_link_libraries(lvgl_plus lvgl lvgl::drivers SDL2)

# options to build examples and tests
option(LVGL_PLUS_BUILD_ALL "Build lvgl_plus tests and examples" ON)
option(LVGL_PLUS_BUILD_STREAMS "Build dart-like streams" ON)
option(LVGL_PLUS_BUILD_TESTS "Build lvgl_plus tests" ON)
option(LVGL_PLUS_BUILD_EXAMPLES "Build lvgl_plus examples" ON)
option(BUILD_GOLDEN_TESTS "Build golden tests" ON)
option(UPDATE_GOLDEN_SAMPLES "Make lvgl_plus golden tests samples" OFF)
set(LVGL_SCREEN_WIDTH 400 CACHE STRING "")
set(LVGL_SCREEN_HEIGHT 300 CACHE STRING "")
add_definitions(-DLVGL_SCREEN_WIDTH=${LVGL_SCREEN_WIDTH})
add_definitions(-DLVGL_SCREEN_HEIGHT=${LVGL_SCREEN_HEIGHT})

if(LVGL_PLUS_BUILD_ALL)
    if(NOT LVGL_PLUS_BUILD_TESTS)
        set(LVGL_PLUS_BUILD_TESTS ON)
        message("LVGL_PLUS_BUILD_TESTS is set to OFF, but LVGL_PLUS_BUILD_ALL forces it to ON")
    endif()
    if(NOT LVGL_PLUS_BUILD_EXAMPLES)
        set(LVGL_PLUS_BUILD_EXAMPLES ON)
        message("LVGL_PLUS_BUILD_EXAMPLES is set to OFF, but LVGL_PLUS_BUILD_ALL forces it to ON")
    endif()
endif()

if (LVGL_PLUS_BUILD_TESTS)
    target_include_directories(lvgl_plus PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/helpers/wait_for")
    # Google tests
    include(GoogleTest)
    enable_testing()
    add_subdirectory(tests)
endif ()

if (LVGL_PLUS_BUILD_STREAMS)
    if (LVGL_PLUS_BUILD_TESTS)
        set(BUILD_TESTING ON)
    endif ()
    add_definitions(-DENABLE_STREAMS="true")
    add_subdirectory(communication)
    target_link_libraries(lvgl_plus streams)
endif ()

if (LVGL_PLUS_BUILD_EXAMPLES)
    if (LVGL_PLUS_BUILD_STREAMS)
        add_subdirectory(examples/heater)
        add_subdirectory(examples/thermostat)
    else ()
        message("Some examples require streams, but they are disabled by LVGL_PLUS_BUILD_STREAMS")
    endif ()
    add_subdirectory(examples/widget_catalog)
    add_subdirectory(examples/multiple_display)
endif ()

if (BUILD_GOLDEN_TESTS)
    target_include_directories(lvgl_plus PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/helpers/snapshot")
    target_include_directories(lvgl_plus PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/helpers/wait_for")
    set(helpers_snapshot
            helpers/wait_for/wait_for.cc
            helpers/snapshot/bitmap.cc
            helpers/snapshot/snapshot.cc)
    target_sources(lvgl_plus PRIVATE ${helpers_snapshot})
endif ()

target_include_directories(lvgl_plus PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/helpers/sdl")

# lvgl
set(LVGL_DIR "${CMAKE_SOURCE_DIR}/lvgl")
add_subdirectory(${LVGL_DIR})
include_directories(${LVGL_DIR})
# drivers
set(LVDRV_DIR "${CMAKE_SOURCE_DIR}/lv_drivers")
add_subdirectory(${LVDRV_DIR})
include_directories(${LVDRV_DIR})
# lvgl needs these defenitions to find a config file
add_definitions(-DLV_LVGL_H_INCLUDE_SIMPLE)
set(LV_CONF_PATH ${CMAKE_SOURCE_DIR}/lv_conf.h CACHE STRING "" FORCE)
add_definitions(-DLV_CONF_PATH=${LV_CONF_PATH})

# headers and sources
set(public_headers
        lv_conf.h
        lv_drv_conf.h
        include/lvgl_plus.hh
        include/lvgl_object.hh
        include/bloc/bloc.hh
        include/bloc/bloc_base.hh
        include/renderable.hh
        include/widget.hh
        include/widget_containers/single_child_widget.hh
        include/widget_containers/multi_child_widget.hh
        include/widget_containers/no_child_widget.hh
        include/widget_container.hh
        include/renderable.hh
        include/text_controllable.hh
        include/controllable.hh
        include/controllers/controller.hh
        include/controllers/text_controller.hh
        include/controllers/text_editing_controller.hh
        # Widgets
        include/widgets/layouts/container.hh
        include/widgets/layouts/row.hh
        include/widgets/layouts/column.hh
        include/widgets/text.hh
        include/widgets/button.hh
        include/widgets/text_field.hh
        include/widgets/switch.hh
        include/widgets/slider.hh
        include/widgets/list_wheel_scroll_view.hh
        include/widgets/dropdown_button.hh
        include/widgets/checkbox.hh
        include/widgets/image.hh
        include/widgets/linear_progress_indicator.hh
        include/widgets/circular_slider.hh
        include/widgets/stream_widget_builder.hh
        include/widgets/bloc_widget_builder.hh
        include/widgets/properties/alignment.hh
        include/widgets/properties/padding.hh
        include/widgets/properties/base/indicator_params.hh
        include/widgets/properties/base/knob_params.hh
        include/widgets/properties/base/range_params.hh
        include/widgets/properties/base/text_basic_params.hh
        )
set(sources
        src/lvgl_plus.cc
        src/lvgl_object.cc
        src/widget.cc
        src/widget_container.cc
        src/mouse_cursor_icon.cc
        # Widgets
        src/widgets/layouts/container.cc
        src/widgets/text.cc
        src/widgets/layouts/column.cc
        src/widgets/button.cc
        src/widgets/text_field.cc
        src/widgets/layouts/row.cc
        src/widgets/switch.cc
        src/widgets/linear_progress_indicator.cc
        src/widgets/checkbox.cc
        src/widgets/circular_slider.cc
        src/widgets/dropdown_button.cc
        src/widgets/list_wheel_scroll_view.cc
        src/widgets/slider.cc
        src/widgets/divider.cc
        src/widgets/image.cc
        src/widgets/table.cc
        src/widgets/chart.cc
        src/widgets/led.cc
        src/widgets/meter.cc
        src/widgets/spinbox.cc
        src/widgets/spinner.cc
        src/widgets/tabview.cc
        src/widgets/list.cc
        src/widgets/message_box.cc
        src/widgets/properties/symbols.cc
        src/widgets/properties/padding.cc
        src/widgets/properties/base/text_basic_params.cc
        src/widgets/properties/theme.cc
        helpers/sdl/sdl_drv.cc
        )
target_sources(lvgl_plus PRIVATE ${sources})
target_include_directories(lvgl_plus PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# TODO clean up install instructions
# export
if (DEFINED LVGL_PLUS_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${LVGL_PLUS_SHARED_LIBS})
endif ()
if (BUILD_SHARED_LIBS)
    set(export_file_name "export_shared.h")
else ()
    set(export_file_name "export_static.h")
endif ()

include(GenerateExportHeader)
generate_export_header(lvgl_plus EXPORT_FILE_NAME include/lvgl_plus/${export_file_name})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})
list(APPEND public_headers
        "${CMAKE_CURRENT_BINARY_DIR}/include/lvgl_plus/${export_file_name}")
list(APPEND sources
        "${CMAKE_CURRENT_BINARY_DIR}/include/lvgl_plus/${export_file_name}")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

target_compile_definitions(lvgl_plus
        PUBLIC
        "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:LVGL_PLUS_STATIC_DEFINE>")

set_target_properties(lvgl_plus PROPERTIES
        PUBLIC_HEADER "${public_headers}"
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

if (LVGL_PLUS_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)

    # LVGL_PLUS_SHARED_LIBS determines static/shared build when defined
    string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
    option(LVGL_PLUS_INSTALL "Generate target for installing lvgl_plus" ${is_top_level})
    set_if_undefined(LVGL_PLUS_INSTALL_CMAKEDIR
            "${CMAKE_INSTALL_LIBDIR}/cmake/lvgl_plus-${PROJECT_VERSION}" CACHE STRING
            "Install path for lvgl_plus package-related CMake files")

    configure_package_config_file(cmake/lvgl_plus-config.cmake.in lvgl_plus-config.cmake
            INSTALL_DESTINATION "${LVGL_PLUS_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(lvgl_plus-config-version.cmake
            COMPATIBILITY SameMajorVersion)

    install(TARGETS lvgl_plus EXPORT lvgl_plus_export
            RUNTIME COMPONENT lvgl_plus
            LIBRARY COMPONENT lvgl_plus NAMELINK_COMPONENT lvgl_plus-dev
            ARCHIVE COMPONENT lvgl_plus-dev
            PUBLIC_HEADER COMPONENT lvgl_plus-dev
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/lvgl_plus")

    set(targets_file "lvgl_plus-shared-targets.cmake")

    if (NOT BUILD_SHARED_LIBS)
        set(targets_file "lvgl_plus-static-targets.cmake")
    endif ()

    install(EXPORT lvgl_plus_export
            COMPONENT lvgl_plus-dev
            FILE "${targets_file}"
            DESTINATION "${LVGL_PLUS_INSTALL_CMAKEDIR}"
            NAMESPACE lvgl_plus::)

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/lvgl_plus-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/lvgl_plus-config-version.cmake"
            COMPONENT lvgl_plus-dev
            DESTINATION "${LVGL_PLUS_INSTALL_CMAKEDIR}")

    if (MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if (BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:lvgl_plus>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else ()
            # TARGET_PDB_FILE does not work for pdb file generated by compiler
            # during static library build, need to determine it another way
            set(pdb_file "$<TARGET_FILE_DIR:lvgl-plus>/$<TARGET_FILE_PREFIX:lvgl-plus>$<TARGET_FILE_BASE_NAME:lvgl-plus>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif ()

        install(FILES "${pdb_file}"
                COMPONENT lvgl-plus-dev
                CONFIGURATIONS Debug RelWithDebInfo
                DESTINATION "${pdb_file_destination}"
                OPTIONAL)
    endif ()
endif ()
