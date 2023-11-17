//
// Created by Feryal Ezgi on 16.11.2023.
//
#include <gtest/gtest.h>
#include "../src/models/ParticleContainer.h"
#include "../src/utils/Generator.h"
#include <spdlog/spdlog.h>

class GeneratorTest : public ::testing::Test {
protected:
    void TeardownParticleContainer() {
        // Remove all particles added during random initialization
        for (auto &randomParticle : cuboidParticles) {
            particleContainer.remove(randomParticle);
        }
        // Clear all
        cuboidParticles.clear();
    }

    std::vector<Particle> cuboidParticles;
    ParticleContainer particleContainer;
};

//Simple test case to check if the correct amount of particles has been added to the container after generating a cuboid
TEST_F(GeneratorTest, CuboidTest) {
    spdlog::info("Starting CuboidTest");

    std::array<double, 3> position = {0.0, 0.0, 0.0};
    std::array<double, 3> size = {2, 2, 2};
    double meshWidth = 1.0;
    std::array<double, 3> velocity = {1.0, 1.0, 1.0};
    double mass = 1.0;
    int typeId = 1;

    Generator::cuboid(particleContainer, position, size, meshWidth, velocity, mass, typeId);

    EXPECT_EQ(particleContainer.size(), size[0] * size[1] * size[2]);

    cuboidParticles = particleContainer.getParticles();
    TeardownParticleContainer();
    ASSERT_TRUE(particleContainer.size() == 0);

    spdlog::info("CuboidTest completed");

}