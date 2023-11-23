include_directories(${PROJECT_SOURCE_DIR}/src
                    ${libxsd_SOURCE_DIR}
                    ${spdlog_SOURCE_DIR}/include)

file(GLOB_RECURSE INTEGRATION_SRC
     "${CMAKE_CURRENT_SOURCE_DIR}/integration/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/integration/*.h")
add_library(integration ${INTEGRATION_SRC})

file(GLOB_RECURSE IO_SRC "${CMAKE_CURRENT_SOURCE_DIR}/io/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/io/*.h")
add_library(io ${IO_SRC})
target_link_libraries(io PUBLIC xerces-c boost_program_options spdlog::spdlog)

file(GLOB_RECURSE PHYSICS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/physics/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/physics/*.h")
add_library(physics ${PHYSICS_SRC})

file(GLOB_RECURSE PARTICLES_SRC "${CMAKE_CURRENT_SOURCE_DIR}/particles/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/particles/*.h")
add_library(particles ${PARTICLES_SRC})

file(GLOB_RECURSE SIMULATION_SRC "${CMAKE_CURRENT_SOURCE_DIR}/simulation/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/simulation/*.h")
add_library(simulation ${SIMULATION_SRC})
target_link_libraries(simulation PRIVATE integration io physics particles)

add_executable(MolSim MolSim.cpp)
target_link_libraries(MolSim PRIVATE simulation)

# activate all compiler warnings. Clean up your code :P depending on the
# compiler different flags are used
target_compile_options(
  MolSim
  PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>
          # disable some spam warnings for icpc...
          $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
