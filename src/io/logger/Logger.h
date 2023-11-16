#pragma once

#include <spdlog/spdlog.h>

#include <memory>

/**
 * @brief Class as wrapper and initializer for a globally usable logger
 */
class Logger {
    static std::shared_ptr<spdlog::logger> init_logger();

   public:
    static std::shared_ptr<spdlog::logger> logger;
};