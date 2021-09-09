cmake_minimum_required(VERSION 3.20)


set(BUILD_SCRIPTS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build_scripts)
file(GLOB build_scripts RELATIVE ${BUILD_SCRIPTS_PATH} ${BUILD_SCRIPTS_PATH}/*)

message(STATUS "Including build scripts: ${build_scripts}")
foreach(script ${build_scripts})
    include(${BUILD_SCRIPTS_PATH}/${script})
endforeach()


set(CMAKE_CXX_STANDARD 20)
project(obps_data_structures)


# enable tests for these targets
list(APPEND TEST_TARGETS
    abstract_factory
    ring_queue
)

if (TEST_TARGETS)
    enable_testing(on)
    FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ds)

