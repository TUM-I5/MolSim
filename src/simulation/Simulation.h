#pragma once

#include <memory>
#include <vector>

#include "integration/IntegrationFunctor.h"
#include "io/IOWrapper.h"
#include "physics/GravitationalForce.h"
#include "types/ParticleContainer.h"

/**
 * @brief Class to run a simulation
 *
 * This class collects all the components needed to run a simulation, and provides a method to run it.
 */
class Simulation {
    std::string input_file;
    IOWrapper io_wrapper;

    std::vector<std::unique_ptr<ForceSource>> force_sources;
    std::unique_ptr<IntegrationFunctor> integration_functor;

    ParticleContainer particle_container;

    double delta_t;
    double end_time;

   public:
    enum class IntegrationMethod {
        VERLET
    };

    /**
     * @brief Construct a new Simulation object and initialize all the necessary components
     * @param input_file Path to the input file
     * @param integration_method Integration method to use (see 'Simulation::IntegrationMethod')
     * @param delta_t Time step per iteration
     * @param end_time End time of the simulation
     */
    Simulation(std::string& input_file, IntegrationMethod integration_method, double delta_t, double end_time);

    /**
     * @brief Runs the simulation with using the parameters given at construction
     */
    void runSimulation();
};