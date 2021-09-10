cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
project(obps_data_structures)

set(MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

# include(LoadSubmodules.cmake)

# add_subdirectory(repo/build_scripts)

# enable tests for these targets
list(APPEND TEST_TARGETS
    abstract_factory
    ring_queue
)

if (TEST_TARGETS)
    enable_testing(on)
    # FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ds)

