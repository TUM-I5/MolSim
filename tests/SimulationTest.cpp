//
// Created by Feryal Ezgi on 2.12.2023.
//
//
// Created by Feryal Ezgi on 2.12.2023.
//
#include "gtest/gtest.h"
#include "Simulation.h"
#include "../src/models/LinkedCellParticleContainer.h"

class SimulationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // for later implementation if needed
    }

    void TearDown() override {
    }
};


TEST_F(SimulationTest, JsonInitializationTest) {
    std::string in = "../input/test/cuboids_reflection.json";
    Simulation simulation(in);

    EXPECT_EQ(simulation.getEndTime(), 2.0);
    EXPECT_EQ(simulation.getDeltaT(), 0.0002);
    EXPECT_EQ(simulation.getVideoDuration(), 30);
    EXPECT_EQ(simulation.getFPS(), 24);
    EXPECT_EQ(simulation.getInputFilePath(), "../input/test/cuboids_reflection.json");
    EXPECT_EQ(simulation.getOutputPath(), "output");
    EXPECT_EQ(simulation.getOutputType(), outputWriter::Disabled);
    // sigma and epsilon aren't stored but rather read and passed on directly, aren't tested here

    // Check the particle container type -> should be LinkedCellParticleContainer
    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    EXPECT_NE(linkedCellParticles, nullptr);

    // the only object stored in the container is a cuboid with the dimensions: [5, 5, 1]
    EXPECT_EQ(linkedCellParticles->size(), 25);
    EXPECT_EQ(particles->size(), 25);

}

