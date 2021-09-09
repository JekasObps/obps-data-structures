cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(AbstractFactory VERSION 1.0)

set(MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

add_library(abstract_factory INTERFACE)

target_include_directories(abstract_factory
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(src/tests ${CMAKE_CURRENT_BINARY_DIR}/tests)