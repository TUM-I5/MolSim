//
// Created by Feryal Ezgi on 16.11.2023.
//
#include <gtest/gtest.h>
#include <random>
#include <spdlog/spdlog.h>
#include "../src/models/ParticleContainer.h"
#include "../src/utils/Generator.h"

class GeneratorTest : public ::testing::Test {
protected:
    void TeardownParticleContainer() {
        // Remove all particles added during random initialization
        for (auto &randomParticle: cuboidParticles) {
            particleContainer.remove(randomParticle);
        }
        // Clear all
        cuboidParticles.clear();
    }

    std::vector<Particle> cuboidParticles;
    ParticleContainer particleContainer;
};



//Simple test case to check if the correct amount of particles has been added to the container after generating a cuboid
TEST_F(GeneratorTest, BasicCuboidTest) {
    spdlog::info("Starting CuboidTest");

    Generator::cuboid(particleContainer, {0.0, 0.0, 0.0}, {2, 2, 2}, 1.0, {1.0, 1.0, 1.0}, 1.0, 1);

    EXPECT_EQ(particleContainer.size(), 2 * 2 * 2);

    cuboidParticles = particleContainer.getParticles();
    TeardownParticleContainer();
    ASSERT_TRUE(particleContainer.size() == 0);

    spdlog::info("CuboidTest completed");
}

// Test cuboid generator with randomized parameters with size comparison
TEST_F(GeneratorTest, RandomizedCuboidTest) {
    spdlog::info("Starting CuboidTest");
    ASSERT_TRUE(particleContainer.size() == 0);

    //Randomized size
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> intDist(1, 50);
    std::array<int, 3> size = {intDist(gen), intDist(gen), intDist(gen)};


    Generator::cuboid(particleContainer, {0.0, 0.0, 0.0}, {size[0], size[1], size[2]}, 1.5, {1.0, 1.0, 1.0}, 1.0, 1);
    int expectedSize = (size[0] * size[1] * size[2]);

    EXPECT_TRUE(particleContainer.size() == expectedSize);

    cuboidParticles = particleContainer.getParticles();
    TeardownParticleContainer();
    // Ensure correct cleanup
    ASSERT_TRUE(particleContainer.size() == 0);

    spdlog::info("CuboidTest completed");
}
