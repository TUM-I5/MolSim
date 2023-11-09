#include <gtest/gtest.h>

#include "simulation/spawners/CuboidSpawner.h"
#include "types/ParticleContainer.h"

/*
 * Test the default constructor of the CuboidSpawner class.
 */
TEST(CuboidSpawner, DefaultConstructor) {
    std::array<double, 3> lower_left_corner = {0, 0, 0};
    std::array<uint, 3> grid_dimensions = {0, 0, 0};
    double grid_spacing = 0;
    double mass = 0;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;
    CuboidSpawner spawner(lower_left_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type);
    EXPECT_EQ(spawner.getLowerLeftCorner(), lower_left_corner);
    EXPECT_EQ(spawner.getGridDimensions(), grid_dimensions);
    EXPECT_EQ(spawner.getGridSpacing(), grid_spacing);
    EXPECT_EQ(spawner.getMass(), mass);
    EXPECT_EQ(spawner.getInitialVelocity(), initial_velocity);
    EXPECT_EQ(spawner.getType(), type);
}
