cmake_minimum_required(VERSION 3.10)



# Define the test executable and list of source files
set(TEST_EXECUTABLE test_MolSim)
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create the test executable
add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})


# Link with necessary libraries (e.g., gtest and your project's source files)
target_link_libraries(${TEST_EXECUTABLE} PRIVATE GTest::gtest_main MolSim_testing)
target_include_directories(${TEST_EXECUTABLE}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libs/libxsd
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${TEST_EXECUTABLE}
        # stuff that is used in headers and source files
        PUBLIC
            xerces-c
)


message("CMAKE_CURRENT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")


add_test(
    NAME ${TEST_EXECUTABLE}
    COMMAND ${TEST_EXECUTABLE}
)
