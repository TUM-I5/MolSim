cmake_policy(SET CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/b10fad38c4026a29ea6561ab15fc4818170d1c10.zip
)
FetchContent_MakeAvailable(googletest)

add_definitions(-DTESTS_SRC_DIR=\"${PROJECT_SOURCE_DIR}/tests\")

