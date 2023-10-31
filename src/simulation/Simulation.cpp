#include "Simulation.h"

#include <iomanip>
#include <iostream>

#include "integration/VerletFunctor.h"
#include "utils/ProgressBar.h"

Simulation::Simulation(std::string& input_file, IntegrationFunctor&& integration_functor, double delta_t, double end_time)
    : input_file(input_file),
      integration_functor(std::move(integration_functor)),
      delta_t(delta_t),
      end_time(end_time) {
    io_wrapper.readFile(input_file, particle_container);
}

void Simulation::runSimulation() {
    std::string output_file_name = "MD_VTK_";
    int iteration = 0;
    double curr_time = 0;

    std::cout << "Running simulation..." << std::endl;

    while (curr_time < end_time) {
        integration_functor.step(particle_container, gravitational_force, delta_t);

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