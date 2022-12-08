#find all test files
file(GLOB_RECURSE MY_TST
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/../tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/../tests/*.h"
        )
list(REMOVE_ITEM MY_TST "${CMAKE_CURRENT_SOURCE_DIR}/../src/MolSim.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/../src/MolSim.h")

#create test target
enable_testing()
find_package(spdlog REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libspdlog)
add_executable(runtest ${MY_TST} TestXMLReader.cpp)
target_include_directories(runtest
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libxsd
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libeigen/Eigen
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libspdlog/include/spdlog
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src
        )
target_link_libraries(runtest PRIVATE pthread PUBLIC xerces-c PUBLIC GTest::gtest_main PUBLIC spdlog::spdlog)
target_compile_features(runtest PUBLIC cxx_std_20)
set_target_properties( runtest
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        )

include(GoogleTest)
gtest_discover_tests(runtest)
