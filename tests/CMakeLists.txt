include(doctest)

#find all test files
file(GLOB_RECURSE MY_TST
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        # header don't need to be included but this might be necessary for some IDEs
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        )

#create test target
add_executable(test)