#include "Simulation.h"

#include <chrono>
#include <iostream>

#include "integration/VerletFunctor.h"
#include "utils/ProgressBar.h"

Simulation::Simulation(ParticleContainer& particles, const std::vector<std::unique_ptr<ForceSource>>& forces, FileOutputHandler& file_output_handler,
                       double delta_t, double simulation_end_time, size_t fps, size_t video_length, IntegrationMethod integration_method)
    : particles(particles),
      delta_t(delta_t),
      simulation_end_time(simulation_end_time),
      file_output_handler(file_output_handler),
      fps(fps),
      video_length(video_length),
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

void Simulation::runSimulation() const {
    int iteration = 0;
    double simulation_time = 0;

    const size_t expected_iterations = simulation_end_time / delta_t;
    const size_t save_every_nth_iteration = std::max(expected_iterations / (fps * video_length), 1ul);

    // keep track of time for progress bar
    auto t_now = std::chrono::system_clock::now();
    auto t_prev = t_now;

    std::cout << "Simulation started..." << std::endl;

    printProgress(0, -1);

    // Calculate initial forces
    particles.applyPairwiseForces(forces);

    while (simulation_time < simulation_end_time) {
        integration_functor->step(particles, forces, delta_t);

        if (iteration % save_every_nth_iteration == 0) {
            // calculate time since last write
            t_now = std::chrono::system_clock::now();
            const double seconds_since_last_write = std::chrono::duration<double>(t_now - t_prev).count();
            t_prev = t_now;

            // calculate estimated remaining time
            const double estimated_remaining_seconds = (expected_iterations - iteration) * seconds_since_last_write / save_every_nth_iteration;

            const size_t percentage = 100 * iteration / expected_iterations;
            printProgress(percentage, estimated_remaining_seconds);

            // write output
            file_output_handler.writeFile(iteration, particles);
        }

        iteration++;
        simulation_time += delta_t;
    }

    printProgress(100, 0);
    std::cout << std::endl;

    std::cout << "Simulation finished." << std::endl;
}