include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE PROJECT_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

list(REMOVE_ITEM PROJECT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/MolSim.cpp")

#print all source files
foreach(_source IN ITEMS ${PROJECT_SRC})
    message(STATUS "Including ${_source}")
endforeach()

add_library(project_lib ${PROJECT_SRC})

target_include_directories(project_lib PUBLIC 
    ${libxsd_SOURCE_DIR}
    ${spdlog_SOURCE_DIR}/include)

target_link_libraries(project_lib PUBLIC xerces-c boost_program_options spdlog::spdlog)

add_executable(MolSim MolSim.cpp)
target_link_libraries(MolSim PUBLIC project_lib)

# activate all compiler warnings.
target_compile_options(
    MolSim PRIVATE 
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>)
