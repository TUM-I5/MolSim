cmake_minimum_required(VERSION 3.3)

# define project name, version, and language
project(PSEMolDyn VERSION 0.0.1 LANGUAGES CXX)

# collect all cpp files
file(GLOB_RECURSE MY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    )

# create make target
add_executable(MolSim ${MY_SRC})

# set cxx standard. You may raise this if you want.
target_compile_features(MolSim PRIVATE cxx_std_11)

target_include_directories(MolSim
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libxsd
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(MolSim xerces-c)

# activate all compiler warnings. Clean up your code :P
target_compile_options(MolSim PRIVATE -Wall)
