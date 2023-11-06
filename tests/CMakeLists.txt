# add googletest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/b10fad38c4026a29ea6561ab15fc4818170d1c10.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/libs/libxsd
)

file(GLOB_RECURSE TEST_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
add_executable(
    tests
    ${TEST_SRC}
)

target_link_libraries(
    tests
    integration
    io
    physics
    simulation
    types
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)