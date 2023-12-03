#pragma once

#include <spdlog/spdlog.h>

#include <memory>

enum class LogType{
    FILE,
    STD
};

/**
 * @brief Class as wrapper and initializer for a globally usable logger
 */
class Logger {


public:
    /**
     * @brief Initializes the logger
     */
    static std::shared_ptr<spdlog::logger> init_logger(LogType log_type = LogType::STD);

    /**
     * @brief Publically accessible shared pointer to the logger
     *
     * @return Logger
     */
    static std::shared_ptr<spdlog::logger> logger;

};