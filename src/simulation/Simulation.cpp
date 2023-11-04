#include "Simulation.h"

#include <iostream>

#include "integration/VerletFunctor.h"
#include "utils/ProgressBar.h"

Simulation::Simulation(std::string& input_file, IntegrationMethod integration_method, double delta_t, double end_time)
    : input_file(input_file),
      delta_t(delta_t),
      end_time(end_time) {
    io_wrapper.readFile(input_file, particle_container);

    force_sources.push_back(std::make_unique<GravitationalForce>());

    switch (integration_method) {
        case IntegrationMethod::VERLET:
            integration_functor = std::make_unique<VerletFunctor>();
            break;
    }
}

void Simulation::runSimulation() {
    std::string output_file_name = "MD_VTK";
    int iteration = 0;
    double curr_time = 0;

    std::cout << "Running simulation..." << std::endl;

    while (curr_time < end_time) {
        integration_functor->step(particle_container, force_sources, delta_t);

        if (iteration % 50 == 0) {
            int percentage = 100 * curr_time / end_time;
            printProgress(percentage);
            io_wrapper.writeFile(output_file_name, iteration, particle_container);
        }

        iteration++;
        curr_time += delta_t;
    }

    printProgress(100);

    std::cout << std::endl
              << "Simulation finished." << std::endl;
}