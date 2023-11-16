# Set download timestamp policy
cmake_policy(SET CMP0135 NEW)

# Enable FetchContent CMake module
include(FetchContent)

# Build spdlog and make the cmake targets available
FetchContent_Declare(
        spdlog
        URL
        # spdlog master:
        # https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.zip
        # spdlog commit 7e635fc (08.07.2023):
        ${PROJECT_SOURCE_DIR}/libs/spdlog.zip
        URL_HASH MD5=acbb04d9c0d4ce56e92c292dc15b1cbc
)

FetchContent_MakeAvailable(spdlog)

