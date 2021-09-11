
subdirlist(ds ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "DataStructures: ${ds}")

foreach(data_structure ${ds})
    add_subdirectory(${data_structure})
endforeach()
