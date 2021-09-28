cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
project(obps_data_structures)

include(Import.cmake)
import_build_scripts()

check_main_project()
check_build()

enable_testing(on)
import_gtest()

# enable tests for these targets
enable_subproject_tests( abstract_factory TEST_ALL)

# enable tests for these targets
enable_subproject_tests( ring_queue TEST_ALL)

# enable tests for these targets
enable_subproject_tests( thread_pool TEST_ALL)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ds)

