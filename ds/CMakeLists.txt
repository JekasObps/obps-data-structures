
MACRO(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(subdirs "")
    foreach(child ${children})
        if (IS_DIRECTORY ${curdir}/${child})
            list(APPEND subdirs ${child})
        endif()
    endforeach()
    set(${result} ${subdirs})
ENDMACRO()

SUBDIRLIST(ds ${CMAKE_CURRENT_SOURCE_DIR})

message("DataStructures: ${ds}")

foreach(data_structure ${ds})
    add_subdirectory(${data_structure})
endforeach()
