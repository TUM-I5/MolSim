#include <iostream>
#include <numeric>

#include "integration/VerletFunctor.h"
#include "io/cli/CLIParser.h"
#include "io/input/FileInputHandler.h"
#include "io/logger/Logger.h"
#include "io/output/FileOutputHandler.h"
#include "particles/ParticleContainer.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "simulation/SimulationParams.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

int main(int argc, char* argsv[]) {
    SimulationParams params_cli = parse_arguments(argc, argsv);

    // Prepare initial conditions for particles
    ParticleContainer initial_particles;
    FileInputHandler file_input_handler;
    SimulationParams params_xml = file_input_handler.readFile(params_cli.input_file_path, initial_particles);

    // Combine parameters from cli and xml
    SimulationParams merged_params = merge_parameters(params_cli, params_xml);

    // Prepare file output handler
    FileOutputHandler file_output_handler{merged_params.output_format, merged_params.output_dir_path};

    // Print Simulation arguments
    Logger::logger->info("Simulation arguments:");
    Logger::logger->info("Input file path: {}", merged_params.input_file_path);
    Logger::logger->info("Output directory path: {}", merged_params.output_dir_path);
    Logger::logger->info("End time: {}", merged_params.end_time);
    Logger::logger->info("Frames per second: {}", merged_params.fps);
    Logger::logger->info("Video length: {}", merged_params.video_length);
    Logger::logger->info("Log level: {}\n", merged_params.log_level);

    if (std::holds_alternative<SimulationParams::DirectSumType>(merged_params.container_type)) {
        Logger::logger->info("Container type: Direct Sum");
    } else {
        Logger::logger->info("Container type: Linked Cells");
        auto lc_container = std::get<SimulationParams::LinkedCellsType>(merged_params.container_type);
        auto domain_size = lc_container.domain_size;
        Logger::logger->info("Domain size: {} x {} x {}", domain_size[0], domain_size[1], domain_size[2]);
        Logger::logger->info("Cutoff radius: {}", lc_container.cutoff_radius);
    }

    // Create all force sources acting on the particles
    std::vector<std::unique_ptr<ForceSource>> forces;

    forces.push_back(std::make_unique<LennardJonesForce>());

    // Print simulation setup
    Logger::logger->info("Simulation setup:");
    Logger::logger->info("Number of particles: {}", initial_particles.size());
    Logger::logger->info("Number of forces: {}", forces.size());

    std::string force_names = "[";
    for (auto& force : forces) {
        force_names += std::string(*force) + ", ";
    }
    force_names += "]";
    Logger::logger->info("Force names: {}", force_names);

    // Initialize simulation
    Simulation simulation{
        initial_particles,         forces, file_output_handler, merged_params.delta_t, merged_params.end_time, merged_params.fps,
        merged_params.video_length};

    simulation.runSimulation();

    return 0;
}
