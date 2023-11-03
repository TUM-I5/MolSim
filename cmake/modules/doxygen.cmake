# make doc_doxygen optional if someone does not have / like doxygen

# TODO: create CMake build option for the target.

# TODO: Add a custom target for building the documentation.

#Option to enable/disable Doxygen
option(BUILD_DOCUMENTATION "Generate Doxygen documentation" ON)

if(BUILD_DOCUMENTATION)
    find_package(Doxygen)

    if(DOXYGEN_FOUND)

        #Add custom target
        add_custom_target(doc_doxygen
                COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "Generating Doxygen documentation"
        )

        #Exclude from default build
        set_target_properties(doc_doxygen PROPERTIES EXCLUDE_FROM_ALL TRUE)

    else()
        message(WARNING "Doxygen not found. Unable to generate documentation.")
    endif()
endif()
