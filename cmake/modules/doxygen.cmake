option ( BUILD_DOC_DOXYGEN "Build the doxygen documentation" OFF )

if(BUILD_DOC_DOXYGEN)
    message(STATUS "Doxygen documentation generation is enabled.")
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        configure_file(${CMAKE_SOURCE_DIR}/Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

        # Create a custom target named 'doc_doxygen' for running Doxygen
        add_custom_target(doc_doxygen
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
