cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(ring_queue VERSION 1.0)

set(MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

add_library(ring_queue INTERFACE)

target_include_directories(ring_queue
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ring_queue INTERFACE spinlock
)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests 
    ${CMAKE_CURRENT_BINARY_DIR}/tests
)