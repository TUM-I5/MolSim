//
// Created by Feryal Ezgi on 16.11.2023.
//
#include <gtest/gtest.h>
#include "../src/models/ParticleContainer.h"
#include "../src/utils/Generator.h"

class GeneratorTest : public ::testing::Test {
protected:
    // For future implementation
};

//Simple test case to check if the correct amount of particles has been added to the container after generating a cuboid
TEST_F(GeneratorTest, CuboidTest) {
    ParticleContainer container;

    std::array<double, 3> position = {0.0, 0.0, 0.0};
    std::array<double, 3> size = {2, 2, 2};
    double meshWidth = 1.0;
    std::array<double, 3> velocity = {1.0, 1.0, 1.0};
    double mass = 1.0;
    int typeId = 1;

    Generator::cuboid(container, position, size, meshWidth, velocity, mass, typeId);

    ASSERT_EQ(container.size(), size[0] * size[1] * size[2]);
}