cmake_minimum_required(VERSION 3.10)




# Define the test executable and list of source files
set(TEST_WEEK1 week1_test)
file(GLOB_RECURSE SOURCES_WEEK1 "${CMAKE_CURRENT_SOURCE_DIR}/week1/*.cpp")

set(TEST_WEEK2 week2_test)
file(GLOB_RECURSE SOURCES_WEEK2 "${CMAKE_CURRENT_SOURCE_DIR}/week2/*.cpp")

set(TEST_WEEK3 week3_test)
file(GLOB_RECURSE SOURCES_WEEK3 "${CMAKE_CURRENT_SOURCE_DIR}/week3/*.cpp")

set(TEST_WEEK4 week4_test)
file(GLOB_RECURSE SOURCES_WEEK4 "${CMAKE_CURRENT_SOURCE_DIR}/week4/*.cpp")

# Create the test executable
add_executable(${TEST_WEEK1} ${SOURCES_WEEK1})

add_executable(${TEST_WEEK2} ${SOURCES_WEEK2})

add_executable(${TEST_WEEK3} ${SOURCES_WEEK3})

add_executable(${TEST_WEEK4} ${SOURCES_WEEK4})


# Link with necessary libraries (e.g., gtest and your project's source files)
target_link_libraries(${TEST_WEEK1} PRIVATE GTest::gtest_main GTest::gmock GTest::gmock_main MolSim_testing)
target_link_libraries(${TEST_WEEK2} PRIVATE GTest::gtest_main GTest::gmock GTest::gmock_main MolSim_testing)
target_link_libraries(${TEST_WEEK3} PRIVATE GTest::gtest_main GTest::gmock GTest::gmock_main MolSim_testing)
target_link_libraries(${TEST_WEEK4} PRIVATE GTest::gtest_main GTest::gmock GTest::gmock_main MolSim_testing)

target_include_directories(${TEST_WEEK1}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libs/libxsd
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)


target_include_directories(${TEST_WEEK2}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libs/libxsd
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)


target_include_directories(${TEST_WEEK3}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libs/libxsd
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_include_directories(${TEST_WEEK4}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libs/libxsd
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)


add_test(
    NAME ${TEST_WEEK1}
    COMMAND ${TEST_WEEK1}
)


add_test(
    NAME ${TEST_WEEK2}
    COMMAND ${TEST_WEEK2}
)

add_test(
    NAME ${TEST_WEEK3}
    COMMAND ${TEST_WEEK3}
)

add_test(
    NAME ${TEST_WEEK4}
    COMMAND ${TEST_WEEK4}
)