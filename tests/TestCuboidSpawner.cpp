#include <gtest/gtest.h>

#include "simulation/spawners/CuboidSpawner.h"
#include "types/ParticleContainer.h"

/*
 * Test if a CuboidSpawner spawns the correct number of particles.
 */
TEST(CuboidSpawner, SpawnCorrectNumberOfParticles) {
    std::array<double, 3> lower_left_corner = {0, 0, 0};
    std::array<uint, 3> grid_dimensions = {3, 7, 11};
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;
    CuboidSpawner spawner(lower_left_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type);

    ParticleContainer particle_container;
    spawner.spawnParticles(particle_container);

    ASSERT_EQ(particle_container.size(), 3 * 7 * 11);
}
