cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0135 NEW)

# define project name, version
project(PSEMolDyn_GroupF VERSION 0.0.1)

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
            cxx_std_14
)

target_include_directories(MolSim
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(XercesC REQUIRED)

include_directories(
        ${XercesC_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
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

include(FetchContent)

# json
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

# googletest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Fetch the spdlog library from the specified repository and tag
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.12.0
)
# Retrieve properties of the spdlog library
FetchContent_GetProperties(spdlog)

# Check if spdlog is not already populated
if (NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif ()

target_link_libraries(MolSim
        # stuff that is used in headers and source files
        PUBLIC
        ${XercesC_LIBRARY}
        Boost::boost
        Boost::program_options
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        GTest::gtest_main
)

target_link_libraries(MolSim ${Boost_LIBRARY})

# collect all cpp files fir tests
file(GLOB_RECURSE MY_TESTS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
        # header don't need to be included but this might be necessary for some IDEs
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h"
)

add_executable(MolSimTest ${MY_TESTS})

target_include_directories(MolSimTest
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
)

target_link_libraries(MolSimTest
    ${XercesC_LIBRARY}
    Boost::boost
    Boost::program_options
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    GTest::gtest_main
)

enable_testing()

include(GoogleTest)
gtest_discover_tests(MolSimTest)







