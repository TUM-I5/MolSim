# Set download timestamp policy
cmake_policy(SET CMP0135 NEW)

# Enable FetchContent CMake module
include(FetchContent)

# Build libxsd and make the cmake targets available
FetchContent_Declare(
        libxsd
        URL
        # libxsd:
        # https://www.codesynthesis.com/download/xsd/4.2/
        ${PROJECT_SOURCE_DIR}/libs/libxsd-4.2.0.zip
        URL_HASH MD5=2104b1a4b205fea022c6d555b20e3bd3
)

FetchContent_MakeAvailable(libxsd)

