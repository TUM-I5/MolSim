#pragma once

#include <spdlog/spdlog.h>

#include <memory>

/**
 * @brief Class as wrapper and initializer for a globally usable logger
 */
class Logger {
    /**
     * @brief Initializes the logger
     */
    static std::shared_ptr<spdlog::logger> init_logger();

   public:
    /**
     * @brief Publically accessible shared pointer to the logger
     *
     * @return Logger
     */
    static std::shared_ptr<spdlog::logger> logger;
};