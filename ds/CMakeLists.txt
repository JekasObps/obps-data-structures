# listing directory
macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(subdirs "")
    foreach(child ${children})
        if (IS_DIRECTORY ${curdir}/${child})
            list(APPEND subdirs ${child})
        endif()
    endforeach()
    set(${result} ${subdirs})
endmacro()

SUBDIRLIST(ds ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "DataStructures: ${ds}")

foreach(data_structure ${ds})
    add_subdirectory(${data_structure})
endforeach()
