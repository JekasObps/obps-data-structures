################
### TESTING: ###
################
macro(CHECK_IF_TEST_ENABLED result)
    list(FIND TEST_TARGETS ${PROJECT_NAME} test_enabled)
    if (test_enabled EQUAL -1)
        set(${result} False)
        message(STATUS "Skipping \"${PROJECT_NAME}\" test.")
    else()
        set(${result} True)
        message(STATUS "Listed \"${PROJECT_NAME}\" test.")
    endif()
endmacro()

function(setup_testing)
    CHECK_IF_TEST_ENABLED(test_enabled)
    if (test_enabled)
        message(STATUS "Testing \"${PROJECT_NAME}\"")

        # collecting tests 
        file(GLOB_RECURSE test_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
        
        include(GoogleTest)
        
        foreach(test_source ${test_sources})
            cmake_path(GET test_source FILENAME test_name)
            list(APPEND test_targets ${test_name})

            add_executable(${test_name} ${test_source})
            
            target_link_libraries(${test_name} 
                PRIVATE ${PROJECT_NAME}
                PRIVATE gtest_main
            )
            
            set_target_properties(${test_name} PROPERTIES CXX_STANDARD 20)
            add_test(NAME ${test_name} COMMAND ${test_name})

            gtest_discover_tests(${test_name})
        endforeach()

        if (NOT test_sources)
            message(STATUS "${PROJECT_NAME} tests not found!")
        endif()
        
    endif() # test_enabled
endfunction() # setup_testing