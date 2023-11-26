#include <functional>
#include <iostream>
#include <numeric>

#include "integration/VerletFunctor.h"
#include "io/cli/CLIParser.h"
#include "io/input/FileInputHandler.h"
#include "io/logger/Logger.h"
#include "particles/ParticleContainer.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "simulation/SimulationParams.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

int main(int argc, char* argsv[]) {
    SimulationParams params_cli = parse_arguments(argc, argsv);

    // Prepare file output handler
    FileOutputHandler file_output_handler{FileOutputHandler::OutputFormat::XYZ, params_cli.output_dir_path};

    // Prepare initial conditions for particles
    ParticleContainer initial_particles;
    FileInputHandler file_input_handler;
    SimulationParams params_xml = file_input_handler.readFile(params_cli.input_file_path, initial_particles);

    // Combine parameters from cli and xml
    SimulationParams params = merge_parameters(params_cli, params_xml);

    // Print Simulation arguments
    Logger::logger->info("Simulation arguments:");
    Logger::logger->info("Input file path: {}", params.input_file_path);
    Logger::logger->info("Output directory path: {}", params.output_dir_path);
    Logger::logger->info("End time: {}", params.end_time);
    Logger::logger->info("Frames per second: {}", params.fps);
    Logger::logger->info("Video length: {}", params.video_length);
    Logger::logger->info("Log level: {}\n", params.log_level);

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
    Simulation simulation{initial_particles, forces, file_output_handler, params.delta_t, params.end_time, params.fps, params.video_length};

    simulation.runSimulation();

    return 0;
}
