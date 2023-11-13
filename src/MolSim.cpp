#include <functional>
#include <iostream>
#include <numeric>

#include "integration/VerletFunctor.h"
#include "io/cli/CLIParser.h"
#include "io/input/FileInputHandler.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "types/ParticleContainer.h"

int main(int argc, char* argsv[]) {
    auto [input_file_path, output_dir_path, delta_t, end_time, fps, video_length] = parse_arguments(argc, argsv);

    // Print Simulation arguments
    std::cout << "Simulation arguments:" << std::endl;
    std::cout << "Input file path: " << input_file_path << std::endl;
    std::cout << "Output directory path: " << output_dir_path << std::endl;
    std::cout << "Delta t: " << delta_t << std::endl;
    std::cout << "End time: " << end_time << std::endl;
    std::cout << "Frames per second: " << fps << std::endl;
    std::cout << "Total length: " << video_length << std::endl;
    std::cout << std::endl;

    // Prepare file output handler
    FileOutputHandler file_output_handler{FileOutputHandler::OutputFormat::VTK, output_dir_path};

    // Prepare initial conditions for particles
    ParticleContainer initial_particles;
    FileInputHandler file_input_handler;
    file_input_handler.readFile(input_file_path, initial_particles);

    // Create all force sources acting on the particles
    std::vector<std::unique_ptr<ForceSource>> forces;

    forces.push_back(std::make_unique<LennardJonesForce>());

    // Print simulation setup
    std::cout << "Simulation setup:" << std::endl;
    std::cout << "Number of particles: " << initial_particles.size() << std::endl;
    std::cout << "Number of forces: " << forces.size() << std::endl;
    std::cout << "Forces: [ ";
    std::for_each(forces.begin(), forces.end(), [](const auto& force) {
        std::cout << force->getName() << " ";
    });
    std::cout << "]" << std::endl;
    std::cout << std::endl;

    // Initialize simulation
    Simulation simulation{initial_particles, forces, file_output_handler, delta_t, end_time, fps, video_length};

    simulation.runSimulation();

    return 0;
}
