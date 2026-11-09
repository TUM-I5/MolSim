#pragma once
#include <functional>

#include "ParticleContainer.h"

namespace simulation {
/**
 * @brief configuration parameters for a simulation run
 *
 * contains positive arguments start time, end time, and delta t
 */
struct SimulationSettings {
    double start_time;
    double end_time;
    double delta_t;
};

/**
 * @brief Molecular dynamic simulation
 */
class Simulation {
    ParticleContainer particles_;
    SimulationSettings settings_;

   public:
    Simulation(ParticleContainer container, const SimulationSettings &settings)
        : particles_(std::move(container)), settings_(settings) {}

    /**
     * @brief Calculates each iteration of a simulation run
     *
     * calculates forces, positions, and velocities for each particle each iteration
     * beginning at settings.start_time until settings.end_time, with a step size
     * settings.delta_t
     * @param post_iteration callback invoked after each iteration and once before the first one
     *          Arguments-
     *          current state
     *          iteration count
     *          current time
     */
    void Run(const std::function<void(const ParticleContainer &, int, double)> &post_iteration);
};
}  // namespace simulation
