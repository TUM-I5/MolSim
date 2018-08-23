project(PSEMolDyn)

cmake_minimum_required(VERSION 3.3)

FILE(GLOB_RECURSE MY_SRC
    "src/*.cpp"
    )

include_directories(src)
include_directories(libxsd)

ADD_EXECUTABLE(MolSim ${MY_SRC})

TARGET_LINK_LIBRARIES(MolSim xerces-c)
