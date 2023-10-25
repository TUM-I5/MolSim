# make doc_doxygen optional if someone does not have / like doxygen


option(DOXY_DOC "Documentation" ON)

if(DOXY_DOC)
    find_package(Doxygen)
    # ./Doxyfile, because the working directory is set to PSEMolDyn_GroupB
    #this will also guarantee that the documentation is within the root folder
    add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ./Doxyfile
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM)
endif()

# TODO: create CMake build option for the target.

# TODO: Add a custom target for building the documentation.