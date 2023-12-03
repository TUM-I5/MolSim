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
 * Test if the particle is reflected at the right boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtLeftBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::REFLECTIVE, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {2, 5, 5};
    std::array<double, 3> v1 = {-4, 0, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 0.5;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_GE((*particle_container)[0].getV()[0], 0);
    EXPECT_NEAR((*particle_container)[0].getV()[0], -v1[0], 1e-3);
}

/*
 * Test if the particle is reflected at the right boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtRightBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::OUTFLOW, BC::REFLECTIVE, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {8, 5, 5};
    std::array<double, 3> v1 = {4, 0, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 0.5;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_LE((*particle_container)[0].getV()[0], 0);
    EXPECT_NEAR((*particle_container)[0].getV()[0], -v1[0], 1e-3);
}

/*
 * Test if the particle is reflected at the bottom boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtBottomBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::OUTFLOW, BC::OUTFLOW, BC::REFLECTIVE, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {5, 2, 5};
    std::array<double, 3> v1 = {0, -4, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 0.5;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_GE((*particle_container)[0].getV()[1], 0);
    EXPECT_NEAR((*particle_container)[0].getV()[1], -v1[1], 1e-3);
}

/*
 * Test if the particle is reflected at the top boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtTopBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::REFLECTIVE, BC::OUTFLOW, BC::OUTFLOW};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {5, 8, 5};
    std::array<double, 3> v1 = {0, 4, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 0.5;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_LE((*particle_container)[0].getV()[1], 0);
    EXPECT_NEAR((*particle_container)[0].getV()[1], -v1[1], 1e-3);
}

/*
 * Test if the particle is reflected at the back boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtBackBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::REFLECTIVE, BC::OUTFLOW};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {5, 5, 2};
    std::array<double, 3> v1 = {0, 0, -4};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 0.5;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_GE((*particle_container)[0].getV()[2], 0);
    EXPECT_NEAR((*particle_container)[0].getV()[2], -v1[2], 1e-3);
}

/*
 * Test if the particle is reflected at the front boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtFrontBoundary) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 5;
    std::array<BC, 6> bcs = {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::REFLECTIVE};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {5, 5, 8};
    std::array<double, 3> v1 = {0, 0, 4};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 0.5;
    params.delta_t = 0.005;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_LE((*particle_container)[0].getV()[2], 0);
    EXPECT_NEAR((*particle_container)[0].getV()[2], -v1[2], 1e-3);
}

/*
 * Test if the particle stays inside the container if bounced along all boundaries.
 */
TEST(ReflectiveBoundaryLC, ParticleStaysInsideContainer) {
    std::array<double, 3> domain_size = {10, 10, 10};
    double cutoff_radius = 10;
    std::array<BC, 6> bcs = {BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE, BC::REFLECTIVE};
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, bcs);

    std::array<double, 3> x1 = {5, 7, 7.4};
    std::array<double, 3> v1 = {2.32, 3.12, 3.23};

    auto particle = Particle(x1, v1, 1, 0);

    particle_container->addParticle(particle);

    std::vector<std::unique_ptr<ForceSource>> forces;
    forces.push_back(std::make_unique<LennardJonesForce>());

    SimulationParams params = TEST_DEFAULT_PARAMS;
    params.end_time = 10;
    params.delta_t = 0.005;
    params.fps = 5;
    Simulation simulation(particle_container, forces, params);

    simulation.runSimulation();

    EXPECT_GE((*particle_container)[0].getX()[0], 0);
    EXPECT_LE((*particle_container)[0].getX()[0], domain_size[0]);
    EXPECT_GE((*particle_container)[0].getX()[1], 0);
    EXPECT_LE((*particle_container)[0].getX()[1], domain_size[1]);
    EXPECT_GE((*particle_container)[0].getX()[2], 0);
    EXPECT_LE((*particle_container)[0].getX()[2], domain_size[2]);
}