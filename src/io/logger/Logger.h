#pragma once

#include <memory>

#include "spdlog/spdlog.h"

class Logger {
    static std::shared_ptr<spdlog::logger> init_logger();
   public:
    static std::shared_ptr<spdlog::logger> logger;
};