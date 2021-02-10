cmake_minimum_required(VERSION 3.16)

project(ZephyrRTOSPlugin)

add_library (zephyr_rtos MODULE zephyr_plugin.c)

if (NOT MSVC)
    # lots of warnings and all warnings as errors
    target_compile_options(zephyr_rtos PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_include_directories (zephyr_rtos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/JLinkGDBServer_RTOSPlugin_SDK_V120)

add_subdirectory(unittests)
