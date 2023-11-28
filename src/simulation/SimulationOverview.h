#pragma once

#include <chrono>
#include <cstddef>

class SimulationOverview {
   public:
    const double total_time_seconds;
    const double average_time_per_iteration_millis;
    const size_t total_iterations;
    const size_t files_written;
};