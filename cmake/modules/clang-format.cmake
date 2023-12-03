# additional target to perform clang-format run, requires clang-format (sudo apt install clang-format-13)

# get all project files
set(
    INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/src/"
    "${PROJECT_SOURCE_DIR}/tests/"
    "${PROJECT_SOURCE_DIR}/benchmarks/"
)
# reset CF_ALL_SOURCE_FILES
foreach (TMP_PATH ${INCLUDE_DIRS})
    # search for files for each path in INCLUDE_DIRS and append them to CF_ALL_SOURCE_FILES
    file(
        GLOB_RECURSE
        CF_ALL_SOURCE_FILES_TMP
        "${TMP_PATH}/*.cpp"
        "${TMP_PATH}/*.h"
    )
    list(APPEND CF_ALL_SOURCE_FILES ${CF_ALL_SOURCE_FILES_TMP})
endforeach (TMP_PATH)

set(DESIRED_CLANG_FORMAT clang-format-13)
find_program(CLANG_FORMAT NAMES ${DESIRED_CLANG_FORMAT})

# priont clang format version
if (CLANG_FORMAT)
    execute_process(
        COMMAND ${CLANG_FORMAT} --version
        OUTPUT_VARIABLE CLANG_FORMAT_VERSION
    )
    message(STATUS "clang format version: ${CLANG_FORMAT_VERSION}")
endif ()

message(STATUS "clang format target")



if (CLANG_FORMAT)
    message(STATUS "clang format found, added clangformat target")
    set(dummyfiles)
    foreach (_file ${CF_ALL_SOURCE_FILES})
        string(
            REPLACE
                "."
                "_"
                file_cf
                ${_file}
        )
        string(
            REPLACE
                ".."
                "."
                file_cf
                ${file_cf}
        )
        set(file_cf ".dummy/cf/${file_cf}_cf")
        add_custom_command(
            OUTPUT ${file_cf}
            COMMAND
                ${CLANG_FORMAT}
                -style=file
                -i
                ${_file}
            DEPENDS ${_file}
        )
        list(APPEND dummyfiles ${file_cf})
    endforeach ()
    add_custom_command(OUTPUT .dummy/cf/clang_dummy COMMAND true DEPENDS ${dummyfiles})
    add_custom_target(clangformat DEPENDS .dummy/cf/clang_dummy)
else ()
    message(
        STATUS
            "${DESIRED_CLANG_FORMAT} not found, not adding clang format target. Other Versions not supported!"
    )
endif ()