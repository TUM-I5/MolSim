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
        ${PROJECT_SOURCE_DIR}/libs/libxsd.zip
        URL_HASH MD5=ce2a1e5715622ecc46099c2dbf63924a
)

FetchContent_MakeAvailable(libxsd)

