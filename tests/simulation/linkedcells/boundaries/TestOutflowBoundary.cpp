#include <gtest/gtest.h>

#include <memory>

#include "io/output/FileOutputHandler.h"
#include "particles/containers/ParticleContainer.h"
#include "particles/containers/linkedcells/LinkedCellsContainer.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "simulation/SimulationUtils.h"
#include "utils/ArrayUtils.h"

using BC = LinkedCellsContainer::BoundaryCondition;

/*
 * Test if the particle is deleted at the left boundary.
 */
TEST(OutflowBoundaryLC, DeletedAtLeftBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::OUTFLOW, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {2, 5, 5};
    std::array<double, 3> v1 = {-4, 0, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 1;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_EQ(particle_container->size(), 0);
}

/*
 * Test if the particle is deleted at the right boundary.
 */
TEST(OutflowBoundaryLC, DeletedAtRightBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::REFLECTIVE, BC::OUTFLOW, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {8, 5, 5};
    std::array<double, 3> v1 = {4, 0, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 1;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_EQ(particle_container->size(), 0);
}

/*
 * Test if the particle is deleted at the bottom boundary.
 */
TEST(OutflowBoundaryLC, DeletedAtBottomBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::REFLECTIVE, BC::REFLECTIVE, BC::OUTFLOW, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {5, 2, 5};
    std::array<double, 3> v1 = {0, -4, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 1;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_EQ(particle_container->size(), 0);
}

/*
 * Test if the particle is deleted at the top boundary.
 */
TEST(OutflowBoundaryLC, DeletedAtTopBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::OUTFLOW, BC::REFLECTIVE, BC::REFLECTIVE};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {5, 8, 5};
    std::array<double, 3> v1 = {0, 4, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 1;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_EQ(particle_container->size(), 0);
}

/*
 * Test if the particle is deleted at the back boundary.
 */
TEST(OutflowBoundaryLC, DeletedAtFrontBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::OUTFLOW, BC::REFLECTIVE};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {5, 5, 2};
    std::array<double, 3> v1 = {0, 0, -4};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 1;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_EQ(particle_container->size(), 0);
}

/*
 * Test if the particle is deleted at the front boundary.
 */
TEST(OutflowBoundaryLC, DeletedAtBackBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::OUTFLOW};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {5, 5, 8};
    std::array<double, 3> v1 = {0, 0, 4};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 1;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_EQ(particle_container->size(), 0);
}
