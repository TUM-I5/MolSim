#include <gtest/gtest.h>

#include "simulation/Simulation.h"
#include "simulation/spawners/CuboidSpawner.h"
#include "types/ParticleContainer.h"

/*
 * Test if the particles of a gravity simulation attract each other.
 */
TEST(GravitySimulation, ParticlesAttractEachother) {
    ParticleContainer particle_container;

    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {0, 0, 0};

    std::array<double, 3> x2 = {1, 0, 0};
    std::array<double, 3> v2 = {0, 0, 0};

    double mass = 1;

    auto p1 = Particle(x1, v1, mass, 0);
    auto p2 = Particle(x2, v2, mass, 0);

    particle_container.addParticle(p1);
    particle_container.addParticle(p2);

    EXPECT_TRUE(false);
}
