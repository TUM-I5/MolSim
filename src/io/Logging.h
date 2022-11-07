//
// Created by alex on 07.11.2022.
//

#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>

namespace loggers {
    extern std::shared_ptr<spdlog::logger> simulation;
    extern std::shared_ptr<spdlog::logger> general;
    extern std::shared_ptr<spdlog::logger> test;

    /**
     * Initializes all logging functionality
     * */
    void init();
}