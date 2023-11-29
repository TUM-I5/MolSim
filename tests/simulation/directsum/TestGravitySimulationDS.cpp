#include <gtest/gtest.h>

#include <memory>

#include "io/output/FileOutputHandler.h"
#include "particles/containers/directsum/DirectSumContainer.h"
#include "particles/containers/linkedcells/LinkedCellsContainer.h"
#include "physics/GravitationalForce.h"
#include "simulation/Simulation.h"
#include "utils/ArrayUtils.h"
#include "simulation/SimulationUtils.h"

/*
 * Test if the particles of a gravity simulation attract each other.
 */
TEST(SimulationRunnerDirectSum, ParticlesAttractEachother_Gravity) {
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<DirectSumContainer>();

    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {0, 0, 0};

    std::array<double, 3> x2 = {1, 1, 1};
    std::array<double, 3> v2 = {0, 0, 0};

    auto initial_distance = ArrayUtils::L2Norm(x1 - x2);

    double mass = 1;

    auto p1 = Particle(x1, v1, mass, 0);
    auto p2 = Particle(x2, v2, mass, 0);

    particle_container->addParticle(p1);
    particle_container->addParticle(p2);

    FileOutputHandler file_output_handler(FileOutputHandler::OutputFormat::NONE);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<GravitationalForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 0.1;
    params.delta_t = 0.001;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    auto new_dist = ArrayUtils::L2Norm((*particle_container)[0].getX() - (*particle_container)[1].getX());

    EXPECT_LT(new_dist, initial_distance);
}
