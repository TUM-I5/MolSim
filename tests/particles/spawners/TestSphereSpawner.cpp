#include <gtest/gtest.h>

#include "particles/containers/directsum/DirectSumContainer.h"
#include "particles/spawners/sphere/SphereSpawner.h"
#include "utils/ArrayUtils.h"

/*
 * Macro to check if a point is in a list of points.
 */
#define EXPECT_CONTAINS_POS_NEAR(list, point, tol) \
    EXPECT_TRUE(std::find_if(list.begin(), list.end(), [&](auto& x) { return ArrayUtils::L2Norm(x - point) < tol; }) != list.end());

/*
 * Test if a SphereSpawner spawns the correct number of particles.
 */
TEST(SphereParticleSpawner, SpawnCorrectNumberOfParticles) {
    std::array<double, 3> center = {0, 0, 0};
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;

    // Radius 1
    std::unique_ptr<ParticleContainer> particle_container1 = std::make_unique<DirectSumContainer>();
    SphereSpawner spawner(center, 1, grid_spacing, mass, initial_velocity, type);
    spawner.spawnParticles(particle_container1);
    EXPECT_EQ(particle_container1->size(), 7);

    // Radius 2
    std::unique_ptr<ParticleContainer> particle_container2 = std::make_unique<DirectSumContainer>();
    SphereSpawner spawner2(center, 2, grid_spacing, mass, initial_velocity, type);
    spawner2.spawnParticles(particle_container2);
    EXPECT_EQ(particle_container2->size(), 33);

    // Radius 9 https://challenges.wolframcloud.com/challenge/lattice-points-in-a-sphere
    std::unique_ptr<ParticleContainer> particle_container9 = std::make_unique<DirectSumContainer>();
    SphereSpawner spawner3(center, 9, grid_spacing, mass, initial_velocity, type);
    spawner3.spawnParticles(particle_container9);
    EXPECT_EQ(particle_container9->size(), 3071);
}

/*
 * Test if a SphereParticleSpawner spawns the particles at the correct positions.
 */
TEST(SphereParticleSpawner, SpawnParticlesAtCorrectPositions) {
    std::array<double, 3> center = {0, 0, 0};
    int radius = 1;
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;
    SphereSpawner spawner(center, radius, grid_spacing, mass, initial_velocity, type);

    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<DirectSumContainer>();
    spawner.spawnParticles(particle_container);

    auto expected_positions =
        std::vector<std::array<double, 3>>({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}});

    for (size_t i = 0; i < particle_container->size(); i++) {
        // check if the position of the particle is inside the expected positions
        EXPECT_CONTAINS_POS_NEAR(expected_positions, (*particle_container)[i].getX(), 1e-10);
    }
}

/*
 * Test if a SphereParticleSpawner returns a good estimate of the actual number of particles.
 */
TEST(SphereParticleSpawner, EstimateNumberOfParticles) {
    std::array<double, 3> center = {0, 0, 0};
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;

    SphereSpawner spawner2(center, 2, grid_spacing, mass, initial_velocity, type);
    std::unique_ptr<ParticleContainer> particle_container2 = std::make_unique<DirectSumContainer>();
    spawner2.spawnParticles(particle_container2);
    EXPECT_NEAR(static_cast<double>(spawner2.getEstimatedNumberOfParticles()) / particle_container2->size(), 1, 0.1);

    SphereSpawner spawner4(center, 4, grid_spacing, mass, initial_velocity, type);
    std::unique_ptr<ParticleContainer> particle_container4 = std::make_unique<DirectSumContainer>();
    spawner4.spawnParticles(particle_container4);
    EXPECT_NEAR(static_cast<double>(spawner4.getEstimatedNumberOfParticles()) / particle_container4->size(), 1, 0.1);

    SphereSpawner spawner8(center, 8, grid_spacing, mass, initial_velocity, type);
    std::unique_ptr<ParticleContainer> particle_container8 = std::make_unique<DirectSumContainer>();
    spawner8.spawnParticles(particle_container8);
    EXPECT_NEAR(static_cast<double>(spawner8.getEstimatedNumberOfParticles()) / particle_container8->size(), 1, 0.1);

    SphereSpawner spawner16(center, 16, grid_spacing, mass, initial_velocity, type);
    std::unique_ptr<ParticleContainer> particle_container16 = std::make_unique<DirectSumContainer>();
    spawner16.spawnParticles(particle_container16);
    EXPECT_NEAR(static_cast<double>(spawner16.getEstimatedNumberOfParticles()) / particle_container16->size(), 1, 0.1);
}