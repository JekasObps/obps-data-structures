cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
project(obps_data_structures)

include(Import.cmake)
import_build_scripts()

check_main_project()
check_build()

# enable tests for these targets
if(NOT TEST_TARGETS)
list(APPEND TEST_TARGETS
    abstract_factory
    ring_queue
)
endif()

if (TEST_TARGETS)
    enable_testing(on)
    import_gtest()
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ds)

