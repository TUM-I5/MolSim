option(BUILD_DOCS "Build Docs using Doxygen" ON)

if (BUILD_DOCS)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        add_custom_target(
                doc_doxygen
                COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    else (DOXYGEN_FOUND)
        message("Documentation cannot be generated: Unable to find Doxygen.")
    endif (DOXYGEN_FOUND)

endif (BUILD_DOCS)