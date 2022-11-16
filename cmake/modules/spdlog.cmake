
if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()


if(SPDLOG_BUILD_EXAMPLE_HO)
    target_link_libraries(MolSim PRIVATE spdlog::spdlog_header_only)
endif()
