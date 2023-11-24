#include <gtest/gtest.h>

#include "io/particle_spawners/CuboidSpawner.h"
#include "particles/containers/ParticleContainer.h"
#include "particles/containers/directsum/DirectSumContainer.h"
#include "utils/ArrayUtils.h"

/*
 * Macro to check if a point is in a list of points.
 */
#define EXPECT_CONTAINS_POS_NEAR(list, point, tol) \
    EXPECT_TRUE(std::find_if(list.begin(), list.end(), [&](auto& x) { return ArrayUtils::L2Norm(x - point) < tol; }) != list.end());

/*
 * Test if a CuboidSpawner spawns the correct number of particles.
 */
TEST(CuboidParticleSpawner, SpawnCorrectNumberOfParticles) {
    std::array<double, 3> lower_left_corner = {0, 0, 0};
    std::array<int, 3> grid_dimensions = {3, 7, 11};
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;
    CuboidSpawner spawner(lower_left_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type);

    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<DirectSumContainer>();
    spawner.spawnParticles(particle_container);

    ASSERT_EQ(particle_container->size(), 3 * 7 * 11);
}

/*
 * Test if a CuboidSpawner spawns the particles at the correct positions.
 */
TEST(CuboidParticleSpawner, SpawnParticlesAtCorrectPositions) {
    std::array<double, 3> lower_left_corner = {0, 0, 0};
    std::array<int, 3> grid_dimensions = {2, 3, 4};
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;
    CuboidSpawner spawner(lower_left_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type);

    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<DirectSumContainer>();
    spawner.spawnParticles(particle_container);

    auto expected_positions =
        std::vector<std::array<double, 3>>({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}, {0, 2, 0}, {1, 2, 0}, {0, 0, 1}, {1, 0, 1},
                                            {0, 1, 1}, {1, 1, 1}, {0, 2, 1}, {1, 2, 1}, {0, 0, 2}, {1, 0, 2}, {0, 1, 2}, {1, 1, 2},
                                            {0, 2, 2}, {1, 2, 2}, {0, 0, 3}, {1, 0, 3}, {0, 1, 3}, {1, 1, 3}, {0, 2, 3}, {1, 2, 3}});

    for (size_t i = 0; i < particle_container->size(); i++) {
        // check if the position of the particle is inside the expected positions
        EXPECT_CONTAINS_POS_NEAR(expected_positions, (*particle_container)[i].getX(), 1e-10);
    }
}
