#pragma once

#include <memory>
#include <vector>

#include "integration/IntegrationFunctor.h"
#include "io/output/FileOutputHandler.h"
#include "physics/GravitationalForce.h"
#include "types/ParticleContainer.h"

/**
 * @brief Class to run a simulation
 *
 * This class collects all the components needed to run a simulation, and provides a method to run it.
 */
class Simulation {
    ParticleContainer particle_container;
    double delta_t;
    double end_time;

    FileOutputHandler file_output_handler;
    std::vector<std::unique_ptr<ForceSource>> force_sources;
    std::unique_ptr<IntegrationFunctor> integration_functor;

   public:
    enum class IntegrationMethod {
        VERLET
    };

    /**
     * @brief Construct a new Simulation object and initialize all the necessary components
     * @param input_file_path Path to the input file
     * @param output_dir_path Path to the directory in which to save the output
     * @param integration_method Integration method to use (see 'Simulation::IntegrationMethod')
     * @param delta_t Time step per iteration
     * @param end_time End time of the simulation
     */
    Simulation(ParticleContainer& initial_particles, FileOutputHandler& file_output_handler, IntegrationMethod integration_method, double delta_t, double end_time);

    /**
     * @brief Runs the simulation, using the parameters given at construction
     */
    void runSimulation();
};