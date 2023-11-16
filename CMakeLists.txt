cmake_minimum_required(VERSION 3.10)

# define project name, version
project(PSEMolDyn_GroupE VERSION 0.0.1)

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
)

# create make target
add_executable(MolSim ${MY_SRC})

# set cxx standard. You may raise this if you want.
target_compile_features(MolSim
        PRIVATE
        cxx_std_17
)

target_include_directories(MolSim
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(MolSim
        # stuff that is used in headers and source files
        PUBLIC
        xerces-c
)

# activate all compiler warnings. Clean up your code :P
# depending on the compiler different flags are used
target_compile_options(MolSim
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        # disable some spam warnings for icpc...
        $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

include(doxygen)

# include FetchContent for downloading external dependencies.
include(FetchContent)

# fetch and declare GoogleTest as an external dependency.
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# making googletest available
FetchContent_MakeAvailable(googletest)

# include the necessary source files in the ParticleContainerTest
add_executable(
        ParticleContainerTest
        UnitTests/ParticleContainerTest.cpp
        src/Particle.cpp
        src/ParticleContainer.cpp
        src/Formulas.cpp
)

# link the googletest and spdlog to the ParticleContainerTest
target_link_libraries(
        ParticleContainerTest
        GTest::gtest_main
        spdlog::spdlog
)

enable_testing()

# include for test discovery
include(GoogleTest)

# discover tests for ParticleContainerTest and ParticleGeneratorTest
gtest_discover_tests(ParticleContainerTest)

# fetch and declare spdlog as an external dependency.
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.9.2  # Replace with the latest version
)

# make spdlog available
FetchContent_MakeAvailable(spdlog)

# default log level is info, but the user can override it
set(LOG_LEVEL "info" CACHE STRING "Set log level (default: info)")

# make the log level available as a preprocessor definition
target_compile_definitions(MolSim PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_${LOG_LEVEL})

# link the spdlog to MolSim
target_link_libraries(MolSim PRIVATE spdlog::spdlog)
