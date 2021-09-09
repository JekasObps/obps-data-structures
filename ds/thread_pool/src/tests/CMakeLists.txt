################
### TESTING: ###
################

if (${MAIN_PROJECT} OR ${TEST_ALL})
    message("Enable Testing:")
    enable_testing(True)

    # Importing gtest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/0134d73a4902574269ff2e42827f7573d3df08ae.zip
        )
    if (WIN32)
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
    FetchContent_MakeAvailable(googletest)

    # collecting tests 
    file(GLOB_RECURSE test_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    
    include(GoogleTest)
    
    foreach(test_source ${test_sources})
        cmake_path(GET test_source FILENAME test_name)
        list(APPEND test_targets ${test_name})

        add_executable(${test_name} ${test_source})
        target_link_libraries(${test_name} PRIVATE thread_pool
                                      PRIVATE gtest_main)
        set_target_properties(${test_name} PROPERTIES CXX_STANDARD 20)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()

    if (${test_targets})
        gtest_discover_tests(${test_targets})
    else()
        message("Tests not found!")
    endif()
endif() # MAIN_PROJECT