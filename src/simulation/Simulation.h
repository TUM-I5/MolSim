#pragma once

#include <vector>

#include "integration/IntegrationFunctor.h"
#include "io/IOWrapper.h"
#include "physics/GravitationalForce.h"
#include "types/ParticleContainer.h"

/**
 * This class collects all the components needed to run a simulation, and provides a method to run it.
 */
class Simulation {
    std::string input_file;
    IOWrapper io_wrapper;

    GravitationalForce gravitational_force;

    ParticleContainer particle_container;
    IntegrationFunctor&& integration_functor;

    double delta_t;
    double end_time;

   public:
    /**
     * @brief Construct a new Simulation object
     * @param particle_container
     * @param force_sources
     * @param integration_method
     */
    Simulation(std::string& input_file, IntegrationFunctor&& integration_functor, double delta_t, double end_time);

    /**
     * @brief Runs the simulation
     * @param particle_container ParticleContainer containing the particles to be simulated
     * @param force_source ForceSource to be used for the simulation
     * @param time_step Time step to be used for the simulation
     * @param iterations Number of iterations to be simulated
     */
    void runSimulation();
};