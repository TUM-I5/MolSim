# Enable FetchContent CMake module
include(FetchContent)

# Build spdlog and make the cmake targets available
FetchContent_Declare(
        spdlog
        URL
        # spdlog master:
        # https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.zip
        # spdlog commit 7e635fc (15.11.2021):
        ${PROJECT_SOURCE_DIR}/libs/spdlog.zip
        URL_HASH MD5=acbb04d9c0d4ce56e92c292dc15b1cbc
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(spdlog)

