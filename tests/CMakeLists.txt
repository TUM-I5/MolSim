# CMakeLists.txt for tests

# Collect all test source files
file(GLOB_RECURSE TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        )

add_executable(MolSimTest ${TEST_SOURCES})

target_include_directories(MolSimTest
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../src  # Include the source directory for access to source files
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libxsd
        )

target_link_libraries(MolSimTest
        GTest::gtest_main
        GTest::gmock_main
        sources
        )

# Enable testing for this executable
enable_testing()

# Use GoogleTest to discover tests
include(GoogleTest)
gtest_discover_tests(MolSimTest)
