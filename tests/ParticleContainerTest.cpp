//
// Created by Alp Kaan Aksu on 03.11.23.
// Modified by Feryal Ezgi Aşkın 12.11.23
#include <gtest/gtest.h>
#include <random>
#include "../src/models/ParticleContainer.h"
#include "spdlog/spdlog.h"


class ParticleContainerTest : public ::testing::Test {
protected:
    // Set up the container and add 4 particles
    void BasicSetUp()  {
        particle1 = Particle({0, 0, 0}, {1, 0, 0}, 1.0, 1);
        particle2 = Particle({1, 2, 3}, {0, 1, 1}, 3.0, 2);
        particle3 = Particle({0, 6, 0}, {1, 1, 0}, 6.0, 3);
        particle4 = Particle({1, 7, 3}, {0, 1, 1}, 6.0, 4);

        particleContainer.add(particle1);
        particleContainer.add(particle2);
        particleContainer.add(particle3);
        particleContainer.add(particle4);

    }

    // Clean up
    void BasicTearDown()  {
        particleContainer.remove(particle1);
        particleContainer.remove(particle2);
        particleContainer.remove(particle3);
        particleContainer.remove(particle4);
    }

    //Creates 10 pseudo-random particles
    void initializeRandomParticles() {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_real_distribution<double> distributionDouble(-10.0, 10.0);
        std::uniform_int_distribution<int> distributionInt(1, 5);

        for (int i = 0; i < 10; ++i) {
            Particle particle({distributionDouble(gen), distributionDouble(gen), distributionDouble(gen)},
                              {distributionDouble(gen), distributionDouble(gen), distributionDouble(gen)},
                              distributionDouble(gen), distributionInt(gen));

            spdlog::info("Initialized Particle {}: {}", i + 1, particle.toString());

            particleContainer.add(particle);
        }
    }

    void TeardownAfterRandomParticles() {
        // Remove all particles added during random initialization
        for (auto &randomParticle : randomParticles) {
            particleContainer.remove(randomParticle);
        }
        // Clear all
        randomParticles.clear();
    }

    ParticleContainer particleContainer;
    Particle particle1;
    Particle particle2;
    Particle particle3;
    Particle particle4;

    // stores the random particles to remove them from the container later on
    std::vector<Particle> randomParticles;
};

//Simple test case for add(const Particle &particle)
TEST_F(ParticleContainerTest, TestAddParticle) {
    BasicSetUp();
    spdlog::info("Starting TestAddParticle");

    EXPECT_TRUE(particleContainer.size() == 4) << "Container size is incorrect after setup.";
    Particle particle5 = Particle({0, 0, 0}, {1, 1, 0}, 1.0, 1);;
    particleContainer.add(particle5);
    EXPECT_TRUE(particleContainer.size() == 5) << "Container size is incorrect after adding a particle.";

    spdlog::info("TestAddParticle completed");
    BasicTearDown();
}

// Test case to check if removing a particle decrements the size of the container.
TEST_F(ParticleContainerTest, TestRemoveParticle) {
    BasicSetUp();
    spdlog::info("Starting TestRemoveParticle");

    EXPECT_TRUE(particleContainer.size() == 4) << "Container size is incorrect after setup.";
    particleContainer.remove(particle2);
    EXPECT_TRUE(particleContainer.size() == 3) << "Container size is incorrect after removing a particle.";

    spdlog::info("TestRemoveParticle completed");
    BasicTearDown();
}

TEST_F(ParticleContainerTest, TestRemoveNonExistentParticle) {
    BasicSetUp();
    spdlog::info("Starting TestRemoveNonExistentParticle");

    EXPECT_TRUE(particleContainer.size() == 4) << "Container size is incorrect after setup.";
    Particle nonExistentParticle({1.0, 1.0, 1.0}, {1.4, 1.2, 1.2}, 1.0, 5);

    //We did not add this particle to the container but are removing it
    particleContainer.remove(nonExistentParticle);
    EXPECT_EQ(particleContainer.size(), 4) << "Container size changed after attempting to remove a non-existent particle.";
    BasicTearDown();

}


// Test case to check if applying a function to all particles works as expected.
TEST_F(ParticleContainerTest, TestApplyToAll) {
    BasicSetUp();
    spdlog::info("Starting TestApplyToAll");

    // Map that stores all particles and their key, which is a boolean value to track if all particles are processed by applyToAll
    std::unordered_map<const Particle *, bool> processedMap;

    // applyToAll should set all corresponding keys to true
    particleContainer.applyToAll([&processedMap](Particle &p) {
        processedMap[&p] = true;
    });

    // Check if all particles have been processed.
    for (const auto &particle: particleContainer.getParticles()) {
        EXPECT_TRUE(processedMap[&particle]) << "Function was not applied to all particles!";
        EXPECT_TRUE(processedMap.find(&particle) != processedMap.end()) << "Function was not applied to all particles!";
    }
    spdlog::info("TestApplyToAll completed");
    BasicTearDown();

}

// Test case for adding particles from a JSON configuration
TEST_F(ParticleContainerTest, TestAddFromJson) {
    BasicSetUp();
    spdlog::info("Starting TestAddFromJson");

    EXPECT_EQ(particleContainer.size(), 4) << "Container size is incorrect after setup.";

    // JSON configuration with 2 particles
    json jsonConfig = {
            {
                    {"type", "particle"},
                    {"position", {2, 2, 2}},
                    {"velocity", {0, 0, 0}},
                    {"mass", 2.5},
                    {"type_id", 2}
            },
            {
                    {"type", "particle"},
                    {"position", {1, 2, 2}},
                    {"velocity", {0, 0, 0}},
                    {"mass", 2.5},
                    {"type_id", 3}
            }
    };

    particleContainer.add(jsonConfig);
    EXPECT_EQ(particleContainer.size(), 6) << "Size is incorrect after adding particles from JSON.";

    spdlog::info("TestAddFromJson completed");
    BasicTearDown();
}


// Test case for adding particles from a JSON configuration
TEST_F(ParticleContainerTest, TestAddFromJson2) {
    BasicSetUp();
    spdlog::info("Starting TestAddFromJson2");
    EXPECT_EQ(particleContainer.size(), 4) << "Container size is incorrect after setup.";

    // JSON configuration with a cuboid of dimensions 2 x 2 x 2, i.e. 8 particles
    json jsonConfig = {
            {
                    {"type", "cuboid"},
                    {"position", {1, 1, 1}},
                    {"size", {2, 2, 2}},
                    {"mesh_width", 0.1},
                    {"velocity", {1, 0, 0}},
                    {"mass", 5.0},
                    {"type_id", 4}
            }

    };
    particleContainer.add(jsonConfig);

    //4 initial particles + 8 particles from the cuboid
    EXPECT_EQ(particleContainer.size(), 12) << "Size is incorrect after adding particles from JSON.";
    spdlog::info("TestAddFromJson2 completed");
    BasicTearDown();

}

// Test case for ApplyToAllPairsOnce using a counter
TEST_F(ParticleContainerTest, TestApplyToAllPairsOnce) {
    BasicSetUp();
    spdlog::info("Starting TestApplyToAllPairsOnce");

    int functionCallCount = 0;

    // Function to increment the counter and log the particle indices
    auto testFunction = [&functionCallCount](Particle &p1, Particle &p2) {
        functionCallCount++;
        spdlog::info("Function called for particles of the type {} and {}", p1.getType(), p2.getType());
    };

    // Apply the function to all pairs of particles
    particleContainer.applyToAllPairsOnce(testFunction);

    // Calculate the expected number of function calls/ pairs
    // for N particles!
    int expectedCalls = particleContainer.size() * (particleContainer.size() - 1) / 2;
    EXPECT_EQ(functionCallCount, expectedCalls) << "Function is not called for all unique pairs of particles.";
    spdlog::info("TestApplyToAllPairsOnce completed");
    BasicTearDown();

}

// Test case for ApplyToAllPairs using a counter approach
TEST_F(ParticleContainerTest, TestApplyToAllPairs) {
    BasicSetUp();
    spdlog::info("Starting TestApplyToAllPairs");

    int functionCallCount = 0;
    auto testFunction = [&functionCallCount](Particle &p1, Particle &p2) {
        functionCallCount++;
        spdlog::info("Function called for particles of the type {} and {}", p1.getType(), p2.getType());
    };
    particleContainer.applyToAllPairs(testFunction);
    int expectedCalls = particleContainer.size() * (particleContainer.size() - 1);
    EXPECT_EQ(functionCallCount, expectedCalls) << "Function is not called for all unique pairs of particles.";
    spdlog::info("TestApplyToAllPairs completed");
    BasicTearDown();

}

// Test case for add(const Particle &particle) with random particles
TEST_F(ParticleContainerTest, TestAddRandomParticles) {
    initializeRandomParticles();
    spdlog::info("Starting TestAddRandomParticles");
    EXPECT_TRUE(particleContainer.size() == 10);
    randomParticles = particleContainer.getParticles();
    TeardownAfterRandomParticles();
    ASSERT_TRUE(particleContainer.size() == 0);
    spdlog::info("TestAddRandomParticles completed");
}






