#include <functional>
#include <iostream>
#include <numeric>

#include "containers/DirectSumContainer.h"
#include "integration/VerletFunctor.h"
#include "io/cli/CLIParser.h"
#include "io/input/FileInputHandler.h"
#include "io/logger/Logger.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

int main(int argc, char* argsv[]) {
    auto [input_file_path, output_dir_path, delta_t, end_time, fps, video_length, log_level] = parse_arguments(argc, argsv);

    // Print Simulation arguments
    Logger::logger->info("Simulation arguments:");
    Logger::logger->info("Input file path: {}", input_file_path);
    Logger::logger->info("Output directory path: {}", output_dir_path);
    Logger::logger->info("End time: {}", end_time);
    Logger::logger->info("Frames per second: {}", fps);
    Logger::logger->info("Video length: {}", video_length);
    Logger::logger->info("Log level: {}\n", log_level);

    // Prepare file output handler
    FileOutputHandler file_output_handler{FileOutputHandler::OutputFormat::VTK, output_dir_path};

    // Prepare initial conditions for particles
    DirectSumContainer initial_particles;
    FileInputHandler file_input_handler;
    file_input_handler.readFile(input_file_path, initial_particles);

    // Create all force sources acting on the particles
    std::vector<std::unique_ptr<ForceSource>> forces;

    forces.push_back(std::make_unique<LennardJonesForce>());

    // Print simulation setup
    Logger::logger->info("Simulation setup:");
    Logger::logger->info("Number of particles: {}", initial_particles.size());
    Logger::logger->info("Number of forces: {}", forces.size());
    Logger::logger->info("Forces: [ {} ]\n",
                         std::accumulate(forces.begin(), forces.end(), std::string{},
                                         [](const auto& acc, const auto& force) { return acc + std::string(*force) + " "; }));

    // Initialize simulation
    Simulation simulation{initial_particles, forces, file_output_handler, delta_t, end_time, fps, video_length};

    simulation.runSimulation();

    return 0;
}
