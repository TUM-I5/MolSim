include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE BENCHMARK_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

#print all test files
foreach(_source IN ITEMS ${BENCHMARK_SRC})
    message(STATUS "Including ${_source}")
endforeach()

add_executable(benchmarks ${BENCHMARK_SRC})

target_link_libraries(
  benchmarks
  project_lib
  GTest::gtest_main)
