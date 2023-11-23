#include <gtest/gtest.h>

#include <memory>

#include "containers/LinkedCellsContainer.h"
#include "containers/ParticleContainer.h"
#include "io/logger/Logger.h"
#include "io/output/FileOutputHandler.h"
#include "physics/GravitationalForce.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "utils/ArrayUtils.h"

/*
 * Test if the particles of a gravity simulation attract each other.
 */
TEST(SimulationRunnerLinkedCells, 2000x2000SquareLC) {
    Logger::logger->set_level(spdlog::level::info);

    std::array<double, 3> domain_size = {4000, 4000, 3};
    double cutoff_radius = 100;

    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius);

    for (int i = 1000; i < 3000; i+=40) {
        for (int j = 1000; j < 3000; j+=40) {
            Particle particle{{i + 0.0, j + 0.0, 1.5}, {0, 0, 0}, 1, 1};
            particle_container->addParticle(particle);
        }
    }

    FileOutputHandler file_output_handler(FileOutputHandler::OutputFormat::VTK);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    Simulation simulation(particle_container, forces, file_output_handler, 0.1, 10, 10);

    // auto start_time = std::chrono::high_resolution_clock::now();
    simulation.runSimulation();
    // auto end_time = std::chrono::high_resolution_clock::now();
}
