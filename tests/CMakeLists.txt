include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE TEST_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

enable_testing()

add_executable(tests ${TEST_SRC})

target_link_libraries(
        tests
        project_lib
        GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

target_compile_definitions(tests PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
