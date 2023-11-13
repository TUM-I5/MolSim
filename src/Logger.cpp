//
// Created by sophy on 13.11.2023.
//

#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

void Logger::init() {
    // Set the log pattern as desired
    spdlog::set_pattern("%^[%T] %n: %v%$");

    // Create and set the core logger
    s_CoreLogger = spdlog::stdout_color_mt("APP");
    s_CoreLogger->set_level(spdlog::level::trace);

    // Create and set the client logger
    s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
    s_ClientLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logger::getCoreLogger() {
    return s_CoreLogger;
}

std::shared_ptr<spdlog::logger>& Logger::getClientLogger() {
    return s_ClientLogger;
}

