#include <iostream>
#include <numeric>

#include "integration/VerletFunctor.h"
#include "io/cli/CLIParser.h"
#include "io/input/FileInputHandler.h"
#include "io/logger/Logger.h"
#include "io/output/FileOutputHandler.h"
#include "particles/containers/ParticleContainer.h"
#include "particles/containers/directsum/DirectSumContainer.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "simulation/SimulationParams.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

void print_simulation_info(const SimulationParams& simulation_params, const ParticleContainer& initial_particles,
                           const std::vector<std::unique_ptr<ForceSource>>& forces);

int main(int argc, char* argsv[]) {
    // Prepare initial conditions for particles
    std::unique_ptr<ParticleContainer> initial_particles = std::make_unique<DirectSumContainer>();

    // Parse CLI arguments
    SimulationParams params_cli = parse_arguments(argc, argsv);

    // Parse input file
    SimulationParams params_xml = FileInputHandler::readFile(params_cli.input_file_path, initial_particles);

    // Combine parameters from CLI and input file
    SimulationParams simulation_params = merge_parameters(params_cli, params_xml);

    // Prepare file output handler
    FileOutputHandler file_output_handler{simulation_params.output_format, simulation_params.output_dir_path};

    // Create all force sources acting on the particles
    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    // Print simulation info
    print_simulation_info(simulation_params, initial_particles, forces);

    // Initialize simulation
    Simulation simulation{initial_particles, forces, simulation_params};

    // Run simulation
    simulation.runSimulation();

    return 0;
}

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

void print_simulation_info(const SimulationParams& simulation_params, const ParticleContainer& initial_particles,
                           const std::vector<std::unique_ptr<ForceSource>>& forces) {
    // Print Simulation arguments
    Logger::logger->info("----------------------------------------");

    Logger::logger->info("Simulation arguments:");
    Logger::logger->info("  Input file path: {}", simulation_params.input_file_path);
    Logger::logger->info("  Output directory path: {}", simulation_params.output_dir_path);
    Logger::logger->info("  End time: {}", simulation_params.end_time);
    Logger::logger->info("  Frames per second: {}", simulation_params.fps);
    Logger::logger->info("  Video length: {}", simulation_params.video_length);

    // Print Physical setup
    Logger::logger->info("Physical setup:");
    Logger::logger->info("  Number of particles: {}", initial_particles.size());
    Logger::logger->info("  Number of forces: {}", forces.size());

    std::string force_names =
        std::accumulate(forces.begin(), forces.end(), std::string{},
                        [](const std::string& acc, const std::unique_ptr<ForceSource>& force) { return acc + std::string(*force) + ", "; });

    Logger::logger->info("  Forces: {}", force_names);

    if (simulation_params.container_type.index() == 1) {
        auto lc_container = std::get<SimulationParams::LinkedCellsType>(simulation_params.container_type);
        auto domain_size = lc_container.domain_size;
        Logger::logger->info("Container type: Linked Cells");
        Logger::logger->info("  Domain size: {} x {} x {}", domain_size[0], domain_size[1], domain_size[2]);
        Logger::logger->info("  Cutoff radius: {}", lc_container.cutoff_radius);
    } else {
        Logger::logger->info("Container type: Direct Sum");
    }

    Logger::logger->info("----------------------------------------\n");
}
