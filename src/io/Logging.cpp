//
// Created by alex on 07.11.2022.
//

#include "io/Logging.h"

namespace loggers {
    std::shared_ptr<spdlog::logger> simulation;
    std::shared_ptr<spdlog::logger> general;
    std::shared_ptr<spdlog::logger> test;

    void init() {
        spdlog::set_pattern("[%H:%M:%S] [%^%l::%n%$] %v");
        simulation = spdlog::stdout_color_mt("simulation");
        general = spdlog::stdout_color_mt("general");
        test = spdlog::stdout_color_mt("test");
#ifdef DEBUG
        spdlog::set_level(spdlog::level::debug);
#endif
    }
}
