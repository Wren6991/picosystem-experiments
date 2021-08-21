cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pimoroni_picosystem)

include(pico_sdk_import.cmake)

project(picosystem_experiments C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

include_directories(include)
add_subdirectory(libs)
add_subdirectory(apps)
