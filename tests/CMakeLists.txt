include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE TEST_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

#print all test files
foreach(_source IN ITEMS ${TEST_SRC})
    message(STATUS "Including ${_source}")
endforeach()

enable_testing()

add_executable(tests ${TEST_SRC})

target_link_libraries(
  tests
  integration
  io
  physics
  simulation
  particles
  GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

target_compile_definitions(tests PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

# set the C++ standard to C++20
set_target_properties(tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
