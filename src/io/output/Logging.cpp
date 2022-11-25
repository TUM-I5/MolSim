//
// Created by alex on 07.11.2022.
//

#include "Logging.h"

namespace io::output::loggers {
    std::shared_ptr<spdlog::logger> simulation;
    std::shared_ptr<spdlog::logger> general;
    std::shared_ptr<spdlog::logger> test;

    void init(level lv) {
        spdlog::set_pattern("[%H:%M:%S] [%^%l::%n%$] %v");
        simulation = spdlog::stdout_color_mt("simulation");
        general = spdlog::stdout_color_mt("general");
        test = spdlog::stdout_color_mt("test");
        spdlog::set_level(static_cast<spdlog::level::level_enum>(lv));
    }
} // io::output::loggers
