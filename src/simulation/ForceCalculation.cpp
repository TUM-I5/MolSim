
#include "ForceCalculation.h"

ForceCalculation::ForceCalculation() {
  _memoryLogger = spdlog::get("memory_logger");
  _memoryLogger->info("ForceCalculation generated!");
}

ForceCalculation::~ForceCalculation() {
    _memoryLogger->info("ForcCalculation destructed!");
}