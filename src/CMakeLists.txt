# CMakeLists.txt for src

# Collect all source files
file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        )

add_library(sources ${SOURCES})

target_include_directories(sources
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libxsd
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(sources
        ${XercesC_LIBRARY}
        Boost::boost
        Boost::program_options
        Boost::filesystem
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        )

target_compile_options(sources
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
        )
