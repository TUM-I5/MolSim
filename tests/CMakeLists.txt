#find all test files
file(GLOB_RECURSE MY_TST
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/../tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/../tests/*.h"
        )
list(REMOVE_ITEM MY_TST "${CMAKE_CURRENT_SOURCE_DIR}/../src/MolSim.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/../src/MolSim.h")

#create test target
add_executable(test ${MY_TST})
target_include_directories(test
        PUBLIC ${DOCTEST_INCLUDE_DIR}/doctest
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libxsd
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libeigen/Eigen
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        )
target_link_libraries(test
        PRIVATE pthread
        PUBLIC xerces-c
        )
target_compile_features(test PUBLIC cxx_std_20)
