cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(abstract_factory VERSION 1.0)

add_library(abstract_factory INTERFACE)

target_include_directories(abstract_factory
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests 
    ${CMAKE_CURRENT_BINARY_DIR}/tests
)