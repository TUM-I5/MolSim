#pragma once

#include <chrono>
#include <cstddef>

/**
 * @brief Class to store some overview data of an executed simulation
 */
class SimulationOverview {
   public:
    /**
     * @brief Total time the simulation took to execute in seconds (includes time for writing output files and logging)
     */
    const double total_time_seconds;

    /**
     * @brief Average time the simulation took to execute per iteration in milliseconds (includes time for writing output files and logging)
     */
    const double average_time_per_iteration_millis;

    /**
     * @brief Total number of iterations the simulation ran for (includes an initial force calculation)
     */
    const size_t total_iterations;

    /**
     * @brief Number of output files written
     */
    const size_t files_written;
};