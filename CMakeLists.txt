cmake_minimum_required(VERSION 3.10)


# define project name, version
project(PSEMolDyn_GroupB VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TEST "Tests" OFF)

# string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
# set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# message(STATUS "C++ Flags(RELEASE)after: ${CMAKE_CXX_FLAGS_RELEASE}")
# message(STATUS "C Flags: ${CMAKE_C_FLAGS}")
# message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS}")

message(STATUS "BUILD_TEST is set to ${BUILD_TEST}")

include(CTest)
enable_testing()

if(BUILD_TEST)
    add_subdirectory(libs/googletest)
    add_subdirectory(tests)
endif(BUILD_TEST)



add_subdirectory(libs/spdlog)


# let ccmake and cmake-gui offer the default build type options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

# set Release as the default build type if it is not yet set.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# collect all cpp files
file(GLOB_RECURSE MY_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        # header don't need to be included but this might be necessary for some IDEs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)


# create make target
# copy xsd schema into build folder, so the executable will find it when trying to read fro file 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input/parameters.xsd DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

add_executable(MolSim ${MY_SRC})

target_include_directories(MolSim
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src

)

target_link_libraries(MolSim
        # stuff that is used in headers and source files
        spdlog
        xerces-c

)


target_compile_features(MolSim
        PRIVATE
        cxx_std_17
)


# activate all compiler warnings. Clean up your code :P
# depending on the compiler different flags are used
target_compile_options(MolSim
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        # disable some spam warnings for icpc...
        $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
)




if(BUILD_TEST)
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input/parameters.xsd DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests )
        



    add_library(MolSim_testing ${MY_SRC})

    target_include_directories(MolSim_testing
            PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src


    )

    target_link_libraries(MolSim_testing
            # stuff that is used in headers and source files
            spdlog
            xerces-c
    )



endif()





# set cxx standard. You may raise this if you want.














list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

include(doxygen)





