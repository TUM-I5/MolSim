include_directories(${PROJECT_SOURCE_DIR}/src
                    ${libxsd_SOURCE_DIR}
                    ${spdlog_SOURCE_DIR}/include)


file(GLOB_RECURSE TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

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
