#include <functional>
#include <iostream>
#include <numeric>

#include "integration/VerletFunctor.h"
#include "io/cli/CLIParser.h"
#include "io/input/FileInputHandler.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "types/ParticleContainer.h"

int main(int argc, char* argsv[]) {
    auto [input_file_path, output_dir_path, delta_t, end_time, fps, video_length] = parse_arguments(argc, argsv);

    std::shared_ptr<spdlog::logger> general = spdlog::stdout_color_mt("general");
    std::shared_ptr<spdlog::logger> error = spdlog::stderr_color_mt("error");

    spdlog::register_logger(general);
    spdlog::register_logger(error);

    // Print Simulation arguments
    general->info("Simulation arguments:");
    general->info("Input file path: {}", input_file_path);
    general->info("Output directory path: {}", output_dir_path);
    general->info("End time: {}", end_time);
    general->info("Frames per second: {}", fps);
    general->info("Video length: {}", video_length);

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
