#include "Simulation.h"

#include <chrono>
#include <iostream>

#include "integration/VerletFunctor.h"
#include "utils/ProgressBar.h"

Simulation::Simulation(ParticleContainer& particles, const std::vector<std::unique_ptr<ForceSource>>& forces, FileOutputHandler& file_output_handler, double delta_t, double simulation_end_time, size_t fps, size_t video_length, IntegrationMethod integration_method)
    : particles(particles),
      delta_t(delta_t),
      simulation_end_time(simulation_end_time),
      file_output_handler(file_output_handler),
      forces(forces),
      fps(fps),
      video_length(video_length) {
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
    int iteration = 0;
    double simulation_time = 0;

    int expected_iterations = simulation_end_time / delta_t;
    int save_every_nth_iteration = std::max(int(expected_iterations / (fps * video_length)), 1);

    // keep track of time for progress bar
    auto t_now = std::chrono::system_clock::now();
    auto t_prev = t_now;

    std::cout << "Simulation started..." << std::endl;

    printProgress(0, -1);

    while (simulation_time < simulation_end_time) {
        integration_functor->step(particles, forces, delta_t);

        if (iteration % save_every_nth_iteration == 0) {
            // calculate time since last write
            t_now = std::chrono::system_clock::now();
            double seconds_since_last_write = std::chrono::duration<double>(t_now - t_prev).count();
            t_prev = t_now;

            // calculate estimated remaining time
            double estimated_remainig_seconds = (expected_iterations - iteration) * seconds_since_last_write / save_every_nth_iteration;

            int percentage = 100 * iteration / expected_iterations;
            printProgress(percentage, estimated_remainig_seconds);

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