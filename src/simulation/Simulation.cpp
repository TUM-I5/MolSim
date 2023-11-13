#include "Simulation.h"

#include <iostream>

#include "integration/VerletFunctor.h"
#include "utils/ProgressBar.h"

Simulation::Simulation(ParticleContainer& initial_particles, FileOutputHandler& file_output_handler, double delta_t, double end_time, int fps, int video_length, IntegrationMethod integration_method)
    : particle_container(initial_particles),
      delta_t(delta_t),
      end_time(end_time),
      file_output_handler(file_output_handler),
      fps(fps),
      video_length(video_length){
    force_sources.push_back(std::make_unique<GravitationalForce>());

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
    std::string output_file_name = "MD_VTK";
    int iteration = 0;
    double curr_time = 0;
    int total_iterations = int(end_time / delta_t);
    int n = int(total_iterations/(fps * video_length));

    std::cout << "Running simulation..." << std::endl;

    while (curr_time < end_time) {
        integration_functor->step(particle_container, force_sources, delta_t);

        if (iteration % n == 0) {
            int percentage = 100 * curr_time / end_time;
            printProgress(percentage);
            file_output_handler.writeFile(output_file_name, iteration, particle_container);
        }

        iteration++;
        //std::cout << "Iteration: " << iteration << std::endl;
        curr_time += delta_t;
    }

    printProgress(100);

    std::cout << std::endl
              << "Simulation finished." << std::endl;
}