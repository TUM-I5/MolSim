#pragma once
#include <spdlog/spdlog.h>

namespace logging {

/**
 * @brief Configures default logger
 *
 * runtime level can only filter further than the compile time one
 *
 * @param level Minimum level to log
 */
void InitLogging(spdlog::level::level_enum level);
}  // namespace logging
