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




// Reflection tests

TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleReflectiveParticleCount) {
    std::string in = "../input/test/reflective/cuboids_reflection.json";
    Simulation simulation(in);

    // Check the particle container type and ensure it is LinkedCellParticleContainer
    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // "size": [5, 5, 1] -> cuboid with 25 particles
    EXPECT_EQ(linkedCellParticles->size(), 25);
    simulation.run();
    // All boundaries are reflective everything should stay inside the container
    EXPECT_EQ(linkedCellParticles->size(), 25);
}

TEST_F(LinkedCellParticleContainerBehavioralTest, ParticleReflectsAtTopBoundary) {
    std::string in = "../input/test/reflective/simple_reflective_top.json";
    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // input has a single particle moving to the top like this and is very close to the boundary
    //            |
    //    *->     |
    //            |

    EXPECT_EQ(linkedCellParticles->size(), 1);
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 1);
    // Iterate over cells and get the first particle
    Particle particle;
    bool particleFound = false;

    for (const auto& cell : linkedCellParticles->getCells()) {
        for (const auto& p : cell) {
            particle = p;
            particleFound = true;
            break;
        }
        if (particleFound) {
            break;
        }
    }

    // Ensure that one particle is found
    ASSERT_TRUE(particleFound);

    // Check if it is reflected properly: Velocity's direction and magnitude will give it away.
    // The particle was moving along the y-Axis to go right initially, hence getV()[1]

    // Keeping the tolerance (kind of) large is important here as we're dealing with floating point numbers and a single particle's velocity
    // doesn't have to be accurate to the decimal point!
    EXPECT_NEAR(20.0, -particle.getV()[1], 1);
}


