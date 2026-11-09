#include "Logging.h"

namespace logging {
void InitLogging(const spdlog::level::level_enum level) {
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
    spdlog::set_level(level);
}
}  // namespace logging
