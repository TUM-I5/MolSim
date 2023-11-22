#include <gtest/gtest.h>

#include "containers/DirectSumContainer.h"
#include "containers/LinkedCellsContainer.h"
#include "containers/ParticleContainer.h"
#include "io/logger/Logger.h"
#include "io/output/FileOutputHandler.h"
#include "physics/GravitationalForce.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "utils/ArrayUtils.h"

#define EXPECT_ARRAY_NEAR(a, b, tol)  \
    for (int i = 0; i < 3; i++) {     \
        EXPECT_NEAR(a[i], b[i], tol); \
    };

std::vector<Particle> createParticles() {
    std::array<double, 3> x1 = {5.5, 4.8, 3.2};
    std::array<double, 3> v1 = {0.2, 0.1, 0};

    std::array<double, 3> x2 = {5.7, 5.2, 7.4};
    std::array<double, 3> v2 = {-0.1, 0.2, 0};

    std::array<double, 3> x3 = {4.2, 5.6, 5.8};
    std::array<double, 3> v3 = {0.1, -0.2, 0};

    std::array<double, 3> x4 = {6.3, 6.2, 4};
    std::array<double, 3> v4 = {0.1, 0.1, 0};

    return {Particle(x1, v1, 1, 0), Particle(x2, v2, 1, 0), Particle(x3, v3, 1, 0), Particle(x4, v4, 1, 0)};
}

/*
 * Test if the simulation of a system of 4 particles with the DirectSumContainer and the LinkedCellsContainer behaves similar.
 */
TEST(SimulationRunnerDirectSumLinkedCellsComparison, RandomSimulation1) {
    Logger::logger->set_level(spdlog::level::info);

    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;

    std::unique_ptr<ParticleContainer> particle_container_ds = std::make_unique<DirectSumContainer>();
    std::unique_ptr<ParticleContainer> particle_container_lc = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius);

    double delta_t = 0.001;
    double end_t = 1;

    auto particles = createParticles();

    for (auto& particle : particles) {
        particle_container_ds->addParticle(particle);
        particle_container_lc->addParticle(particle);
    }

    FileOutputHandler file_output_handler_ds(FileOutputHandler::OutputFormat::NONE, "./DS_output");
    FileOutputHandler file_output_handler_lc(FileOutputHandler::OutputFormat::NONE, "./LC_output");

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    Simulation simulation_ds(particle_container_ds, forces, file_output_handler_ds, delta_t, end_t, 0);
    Simulation simulation_lc(particle_container_lc, forces, file_output_handler_lc, delta_t, end_t, 0);

    simulation_ds.runSimulation();
    simulation_lc.runSimulation();

    for (int i = 0; i < 4; i++) {
        EXPECT_ARRAY_NEAR((*particle_container_ds)[i].getX(), (*particle_container_lc)[i].getX(), 1e-7);
    }
}

std::vector<Particle> createCollidingCubesParticles() {
    std::vector<Particle> particles;

    std::array<double, 3> offset_center = {5, 5, 5};

    for (double i = -1; i <= 1; i++) {
        for (double j = -1; j <= 1; j++) {
            for (double k = -1; k <= 1; k++) {
                std::array<double, 3> x = {i, j, k};
                std::array<double, 3> v = {0, 0, 0};
                particles.push_back(Particle(x + offset_center, v, 1, 0));
            }
        }
    }

    particles.push_back(Particle({0, 5.5, 5}, {50, 0, 0}, 1, 0));

    return particles;
}

TEST(SimulationRunnerDirectSumLinkedCellsComparison, Collision) {
    Logger::logger->set_level(spdlog::level::info);

    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 20;

    std::unique_ptr<ParticleContainer> particle_container_ds = std::make_unique<DirectSumContainer>();
    std::unique_ptr<ParticleContainer> particle_container_lc = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius);

    double delta_t = 0.0005;
    double end_t = 1;

    auto particles = createCollidingCubesParticles();

    for (auto& particle : particles) {
        particle_container_ds->addParticle(particle);
        particle_container_lc->addParticle(particle);
    }

    FileOutputHandler file_output_handler_ds(FileOutputHandler::OutputFormat::NONE, "./DS_output");
    FileOutputHandler file_output_handler_lc(FileOutputHandler::OutputFormat::NONE, "./LC_output");

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    Simulation simulation_ds(particle_container_ds, forces, file_output_handler_ds, delta_t, end_t, 0);
    Simulation simulation_lc(particle_container_lc, forces, file_output_handler_lc, delta_t, end_t, 0);

    simulation_ds.runSimulation();
    simulation_lc.runSimulation();

    for (int i = 0; i < 4; i++) {
        EXPECT_ARRAY_NEAR((*particle_container_ds)[i].getX(), (*particle_container_lc)[i].getX(), 1e-3);
    }
}