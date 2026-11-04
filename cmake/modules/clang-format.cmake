# Find clang-format
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    message(STATUS "Found clang-format: ${CLANG_FORMAT}")
    # Define list of all source files
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
            "src/*.cpp"
            "src/*.h"
            "tests/*.cpp"
            "tests/*.h"
    )

    # Remove any files in build directories
    list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*/build/.*")

    # Create format target
    add_custom_target(format
            COMMAND ${CLANG_FORMAT}
            -i
            -style=file
            ${ALL_CXX_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Auto-formatting all source files with clang-format"
    )
else()
    message(WARNING "clang-format not found - formatting target not available")
endif()