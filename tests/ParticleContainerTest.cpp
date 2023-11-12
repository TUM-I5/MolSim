//
// Created by Alp Kaan Aksu on 03.11.23.
// Modified by Feryal Ezgi Aşkın 12.11.23

#include <gtest/gtest.h>
#include "../src/models/ParticleContainer.h"
#include "../src/models/ParticleContainer.cpp"
#include "../src/models/Particle.cpp"

class ParticleContainerTest : public ::testing::Test {
protected:
    // Set up the container and add particles
    void SetUp() override {
        particle1 = Particle({0, 0, 0}, {1, 0, 0}, 1.0, 1);
        particle2 = Particle({1, 2, 3}, {0, 1, 1}, 3.0, 2);

        particleContainer.add(particle1);
        particleContainer.add(particle2);
    }

    // Clean up
    void TearDown() override {
        // Possible logging
    }

    ParticleContainer particleContainer;
    Particle particle1;
    Particle particle2;
};

//Test case for add(const Particle &particle) and the setup
TEST_F(ParticleContainerTest, TestAddParticle) {
    EXPECT_TRUE(particleContainer.size() == 2) << "Size is incorrect after adding particles.";
}

// Test case to check if removing a particle decrements the size of the container.
TEST_F(ParticleContainerTest, TestRemoveParticle) {
    particleContainer.remove(particle2);

    EXPECT_EQ(particleContainer.size(), 1);
}

// Test case to check if applying a function to all particles works as expected.
TEST_F(ParticleContainerTest, TestApplyToAll) {
    // Map to store all particles and as their key a boolean value to track if all particles are processed by applyToAll.
    std::unordered_map<const Particle*, bool> processedMap;

    // applyToAll should set all keys to true.
    particleContainer.applyToAll([&processedMap](Particle& p) {
        processedMap[&p] = true;
    });

    // Check if all particles have been processed.
    for (const auto& particle : particleContainer.getParticles()) {
        EXPECT_TRUE(processedMap[&particle]) << "Function was not applied to all particles!";
        EXPECT_TRUE(processedMap.find(&particle) != processedMap.end()) << "Function was not applied to all particles!";
    }
}
