#include "Simulation.h"

#include <chrono>
#include <iostream>

#include "integration/VerletFunctor.h"
#include "utils/ProgressBar.h"

Simulation::Simulation(ParticleContainer& particles, const std::vector<std::unique_ptr<ForceSource>>& forces, FileOutputHandler& file_output_handler, double delta_t, double simulation_end_time, IntegrationMethod integration_method)
    : particles(particles),
      delta_t(delta_t),
      simulation_end_time(simulation_end_time),
      file_output_handler(file_output_handler),
      forces(forces) {
    switch (integration_method) {
        case IntegrationMethod::VERLET:
            integration_functor = std::make_unique<VerletFunctor>();
            break;
        default:
            std::cerr << "Integration method not implemented." << std::endl;
            exit(1);
    }
}

void Simulation::runSimulation() {
    int SAVE_EVERY_NTH_ITERATION = 50;

    int iteration = 0;
    double simulation_time = 0;

    int expected_iterations = simulation_end_time / delta_t;

    // keep track of time for progress bar
    std::time_t t_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::time_t t_prev = t_now;

    std::cout << "Simulation started..." << std::endl;
    std::cout << "Start time: " << std::ctime(&t_now);

    printProgress(0, -1);

    while (simulation_time < simulation_end_time) {
        integration_functor->step(particles, forces, delta_t);

        if (iteration % SAVE_EVERY_NTH_ITERATION == 0) {
            // calculate time since last write
            t_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            double seconds_since_last_write = std::difftime(t_now, t_prev);
            t_prev = t_now;

            // calculate estimated remaining time
            double estimated_remainig_seconds = (expected_iterations - iteration) * seconds_since_last_write / SAVE_EVERY_NTH_ITERATION;

            int percentage = 100 * iteration / expected_iterations;
            printProgress(percentage, estimated_remainig_seconds);

            // write output
            file_output_handler.writeFile(iteration, particles);
        }

        iteration++;
        simulation_time += delta_t;
    }

    t_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    printProgress(100, 0);
    std::cout << std::endl;

    std::cout << "Simulation finished." << std::endl;
    std::cout << "End time: " << std::ctime(&t_now);
}