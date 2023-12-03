//
// Created by Feryal Ezgi on 16.11.2023.
//
#include <gtest/gtest.h>
#include <random>
#include <cmath>
#include <spdlog/spdlog.h>
#include "../src/models/ParticleContainer.h"
#include "../src/utils/Generator.h"
#include "../src/utils/ArrayUtils.h"

class GeneratorTest : public ::testing::Test {
protected:
    void TeardownParticleContainer() {
        // Remove all particles added during random initialization
        for (auto &randomParticle: cuboidParticles) {
            particleContainer.remove(randomParticle);
        }
        cuboidParticles.clear();
    }

    std::vector<Particle> cuboidParticles;
    ParticleContainer particleContainer;

};



// Simple test case to check if the correct amount of particles has been added to the container after generating a cuboid
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

// Checks if the generated sphere is smaller than a cube with the same diameter/dimension by comparing their volumes for various radii
TEST_F(GeneratorTest, BasicSphereSizeTest) {
    spdlog::info("Starting BasicSphereSizeTest");

    for (int radius = 1; radius <= 5; ++radius) {
        Generator::sphere(particleContainer, {0.0, 0.0, 0.0}, radius, 1, {1.2, 2.0, 0.0}, 1.0, 2);
        double maxAmount = pow(2 * radius, 3);
        double actualAmount = particleContainer.size();

        EXPECT_GE(maxAmount, actualAmount);

        cuboidParticles = particleContainer.getParticles();
        TeardownParticleContainer();
        ASSERT_TRUE(particleContainer.size() == 0);
    }
    spdlog::info("BasicSphereSizeTest completed");

}

// Checks if the generated sphere is smaller than a cube with the same diameter/dimension by comparing their volumes for various mesh-widths
TEST_F(GeneratorTest, SphereSizeVariantMeshTest) {
    spdlog::info("Starting SphereSizeVariantMeshTest");

    for (double meshWidth: {1.0, 1.5, 3.5, 2.2}) {
        int radius = 5;
        Generator::sphere(particleContainer, {9.35, 0.5, 2.0}, radius, meshWidth, {1.2, 2.0, 0.0}, 1.0, 2);

        double maxAmount = pow(2 * radius * meshWidth, 3);
        double actualAmount = particleContainer.size();

        EXPECT_GE(maxAmount, actualAmount);

        cuboidParticles = particleContainer.getParticles();
        TeardownParticleContainer();
        ASSERT_TRUE(particleContainer.size() == 0);
    }
    spdlog::info("SphereSizeVariantMeshTest completed");


}

// Tests if the correct amount of particles is generated for a non-integer mesh-width
TEST_F(GeneratorTest, ExactSphereSizeTest) {
    spdlog::info("Starting ExactSphereSizeTest");

    //Source for comparison: https://challenges.wolframcloud.com/challenge/lattice-points-in-a-sphere
    Generator::sphere(particleContainer, {0.0, 0.0, 0.0}, 9, 0.5, {1.2, 2.0, 0.0}, 1.0, 2);
    EXPECT_EQ(particleContainer.size(), 3071);

    cuboidParticles = particleContainer.getParticles();
    TeardownParticleContainer();
    ASSERT_TRUE(particleContainer.size() == 0);
    spdlog::info("ExactSphereSizeTest completed");

}


// Test that verifies the general position of particles in a generated sphere
TEST_F(GeneratorTest, SphereGeneralPositionTest) {
    spdlog::info("Starting SphereGeneralPositionTest");

    int radius = 5;
    double meshWidth = 1.0;
    std::array<double, 3> center = {0.0, 0.0, 0.0};
    Generator::sphere(particleContainer, center, radius, meshWidth, {1.2, 2.0, 0.0}, 1.0, 2);

    cuboidParticles = particleContainer.getParticles();

    for (const Particle &particle: cuboidParticles) {
        double normalizedDistance =
                ArrayUtils::L2Norm(particle.getX() - center) / meshWidth;

        // is the particle inside the sphere boundaries?
        EXPECT_LE(normalizedDistance, radius);
    }

    cuboidParticles = particleContainer.getParticles();
    TeardownParticleContainer();
    ASSERT_TRUE(particleContainer.size() == 0);
    spdlog::info("SphereGeneralPositionTest completed");

}

// Test that checks if the correct particles are generated at exact positions in a sphere
TEST_F(GeneratorTest, SphereExactPositionTest) {
    spdlog::info("Starting SphereExactPositionTest");

    int radius = 1;
    double meshWidth = 1.0;
    std::array<double, 3> center = {0.0, 0.0, 0.0};
    Generator::sphere(particleContainer, center, radius, meshWidth, {1.2, 2.0, 0.0}, 1.0, 2);

    cuboidParticles = particleContainer.getParticles();

    std::vector<std::array<double, 3>> expectedCoordinates = {
            {0.0,  0.0,  0.0},
            {-1.0, 0.0,  0.0},
            {1.0,  0.0,  0.0},
            {0.0,  1.0,  0.0},
            {0.0,  -1.0, 0.0},
            {0.0,  0.0,  1.0},
            {0.0,  0.0,  -1.0}};

    for (const auto &expectedCoord: expectedCoordinates) {
        bool matchFound = false;
        for (const Particle &particle: cuboidParticles) {
            std::array<double, 3> particleCoordinates = particle.getX();

            if (particleCoordinates == expectedCoord) {
                matchFound = true;
                break;
            }
        }
        // Test fails if any one of the sphere's particles don't match any expected coordinate.
        EXPECT_TRUE(matchFound);
    }
    cuboidParticles = particleContainer.getParticles();
    TeardownParticleContainer();
    ASSERT_TRUE(particleContainer.size() == 0);
    spdlog::info("SphereExactPositionTest completed");

}

