#include <iostream>

#include "integration/VerletFunctor.h"
#include "io/cli/CLIParser.h"
#include "io/input/FileInputHandler.h"
#include "physics/GravitationalForce.h"
#include "simulation/Simulation.h"
#include "types/ParticleContainer.h"

int main(int argc, char* argsv[]) {
    auto [input_file_path, output_dir_path, delta_t, end_time] = parse_arguments(argc, argsv);

    // Create initial conditions for particles
    ParticleContainer initial_particles;
    FileInputHandler file_input_handler;
    file_input_handler.readFile(input_file_path, initial_particles);

    // Create all force sources acting on the particles
    std::vector<std::unique_ptr<ForceSource>> forces;

    forces.push_back(std::make_unique<GravitationalForce>());

    // Prepare file output handler
    FileOutputHandler file_output_handler{FileOutputHandler::OutputFormat::VTK, output_dir_path};

    // Initialize simulation
    Simulation simulation{initial_particles, forces, file_output_handler, delta_t, end_time};

    simulation.runSimulation();

    return 0;
}
