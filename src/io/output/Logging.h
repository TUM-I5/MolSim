//
// Created by alex on 07.11.2022.
//

#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>

namespace io::output::loggers {
    extern std::shared_ptr<spdlog::logger> simulation;
    extern std::shared_ptr<spdlog::logger> general;
    extern std::shared_ptr<spdlog::logger> test;

    /**
     * Copy of spdlog::level enum to avaid additional imports.
     * */
    enum level {
        trace = SPDLOG_LEVEL_TRACE,
        debug = SPDLOG_LEVEL_DEBUG,
        info = SPDLOG_LEVEL_INFO,
        warn = SPDLOG_LEVEL_WARN,
        err = SPDLOG_LEVEL_ERROR,
        critical = SPDLOG_LEVEL_CRITICAL,
        off = SPDLOG_LEVEL_OFF,
        level_count
    };

    /**
     * Initializes all logging functionality
     * */
    void init(level lv);
} // io::output::loggers