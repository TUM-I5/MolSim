# make doc_doxygen optional if someone does not have / like doxygen

# TODO: create CMake build option for the target.

option ( BUILD_DOC_DOXYGEN "Build the doxygen documentation" OFF )

# TODO: Add a custom target for building the documentation.

if(BUILD_DOC_DOXYGEN)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        configure_file(Doxyfile Doxyfile @ONLY)

        # Create a custom target named 'docs' for running Doxygen
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile 
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating Doxygen documentation"
            VERBATIM
        )
    else()
        message(WARNING "Doxygen not found. Skipping documentation generation.")
    endif()
else()
    message(STATUS "Doxygen documentation generation is disabled.")
endif()