cmake_minimum_required(VERSION 3.16)

project(ZephyrRTOSPlugin)

include (CTest)
if(BUILD_TESTING)
	set(CMAKE_BUILD_TYPE Debug)
	add_executable(plugin_test plugin_test.c ../zephyr_plugin.c)
    target_include_directories (plugin_test PRIVATE ../JLinkGDBServer_RTOSPlugin_SDK_V120)
	add_test(plugin_test plugin_test)
endif()
