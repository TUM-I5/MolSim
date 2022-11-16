include(FetchContent)
FetchContent_Declare(
    googletest 
    GIT_REPOSITORY https://github.com/google/googletest.git
    #update release version frequently
    GIT_TAG release-1.12.1
) 
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED) 
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()

enable_testing()


file(GLOB_RECURSE PC_TEST_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc"
    # header don't need to be included but this might be necessary for some IDEs
    "${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.h"
)

add_executable(
    AllTests
    ${PC_TEST_SRC}
)

target_include_directories(
    AllTests
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
)

target_link_libraries(
    AllTests
    gtest 
    gmock
    #gtest_main
)

include(GoogleTest)

gtest_discover_tests(AllTests)