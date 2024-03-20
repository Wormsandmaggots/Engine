cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(ENGINE VERSION 2.0)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/settings)


include(global_settings)


add_subdirectory(vendor)


add_subdirectory(code)
