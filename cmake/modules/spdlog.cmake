include(FetchContent)
FetchContent_Declare(
        spdlog
        URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        SYSTEM
)
FetchContent_MakeAvailable(spdlog)

set(LOG_LEVEL "INFO" CACHE STRING "Compile time Logging level: TRACE DEBUG INFO WARN ERROR CRITICAL OFF")

set_property(CACHE LOG_LEVEL PROPERTY STRINGS TRACE DEBUG INFO WARN ERROR CRITICAL OFF)

target_link_libraries(MolSim PRIVATE spdlog::spdlog)
target_compile_definitions(MolSim PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_${LOG_LEVEL})