//
// Created by Feryal Ezgi on 4.12.2023.
//
#include "gtest/gtest.h"
#include "Simulation.h"
#include "../src/models/LinkedCellParticleContainer.h"

class LinkedCellParticleContainerBehavioralTest : public ::testing::Test {

};


// Outflow tests
TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowTop) {
    std::string in = "../input/test/outflow/simple_outflow_top.json";
    Simulation simulation(in);

    // Check the particle container type and ensure it is LinkedCellParticleContainer
    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // a single particle heading upwards (movement parallel to the z-axis)
    EXPECT_EQ(linkedCellParticles->size(), 1);
    simulation.run();
    // should outflow -> meaning it should get deleted from the container
    EXPECT_EQ(linkedCellParticles->size(), 0);
}

TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowBottom) {
    std::string in = "../input/test/outflow/simple_outflow_bottom.json";
    Simulation simulation(in);

    // Check the particle container type and ensure it is LinkedCellParticleContainer
    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // a single particle heading downwards (movement parallel to the z-axis)
    EXPECT_EQ(linkedCellParticles->size(), 1);
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0);
}


TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowRight) {
    std::string in = "../input/test/outflow/simple_outflow_right.json";
    Simulation simulation(in);

    // Check the particle container type and ensure it is LinkedCellParticleContainer
    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // a single particle heading right (movement parallel to the y-axis)
    EXPECT_EQ(linkedCellParticles->size(), 1);
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0);
}


TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowLeft) {
    std::string in = "../input/test/outflow/simple_outflow_left.json";
    Simulation simulation(in);

    // Check the particle container type and ensure it is LinkedCellParticleContainer
    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // a single particle heading left (movement parallel to the y-axis)
    EXPECT_EQ(linkedCellParticles->size(), 1);
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0);
}


TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowFront) {
    std::string in = "../input/test/outflow/simple_outflow_front.json";
    Simulation simulation(in);

    // Check the particle container type and ensure it is LinkedCellParticleContainer
    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);
    EXPECT_EQ(linkedCellParticles->size(), 1);
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0);
}


TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowBack) {
    std::string in = "../input/test/outflow/simple_outflow_back.json";
    Simulation simulation(in);

    // Check the particle container type and ensure it is LinkedCellParticleContainer
    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);
    EXPECT_EQ(linkedCellParticles->size(), 1);
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0);
}

























