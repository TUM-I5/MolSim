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



  # add executable for each benchmark
foreach(_source IN ITEMS ${BENCHMARK_SRC})
    if(${_source} MATCHES ".*\\.h$")
        continue()
    endif()
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(${_name} ${_source})
    target_link_libraries(${_name} project_lib)
endforeach()
  
