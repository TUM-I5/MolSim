//
// Created by Feryal Ezgi on 4.12.2023.
//
#include "gtest/gtest.h"
#include "Simulation.h"
#include <spdlog/spdlog.h>
#include "../src/models/LinkedCellParticleContainer.h"

class LinkedCellParticleContainerBehavioralTest : public ::testing::Test {
protected:
    // finds and returns the first particle in the container
    Particle findFirstParticle(const std::shared_ptr<LinkedCellParticleContainer>& linkedCellParticles) {
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

        // Ensure that a particle has been found
        EXPECT_TRUE(particleFound);
        return particle;
    }
};


// Outflow tests: The test names are pretty self-explanatory :)
// Simulate a scenario with a single particle which can be found in the input json file
TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowTop) {
    std::string in = "../input/test/outflow/simple_outflow_top.json";
    spdlog::info("Starting SimpleOutflowTop test with input file: {}", in);
    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // a single particle heading upwards (movement parallel to the z-axis)
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count before simulation.";
    simulation.run();
    // should outflow -> meaning it should get deleted from the container
    EXPECT_EQ(linkedCellParticles->size(), 0) << "Particle did not outflow as expected.";

    spdlog::info("Final particle count: {}", linkedCellParticles->size());
    spdlog::info("SimpleOutflowTop test completed");
}

TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowBottom) {
    std::string in = "../input/test/outflow/simple_outflow_bottom.json";
    spdlog::info("Starting SimpleOutflowBottom test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // a single particle heading downwards (movement parallel to the z-axis)
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count before simulation.";
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0) << "Particle did not outflow as expected.";

    spdlog::info("Final particle count: {}", linkedCellParticles->size());
    spdlog::info("SimpleOutflowBottom test completed");
}


TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowRight) {
    std::string in = "../input/test/outflow/simple_outflow_right.json";
    spdlog::info("Starting SimpleOutflowRight test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // a single particle heading right (movement parallel to the y-axis)
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count before simulation.";
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0) << "Particle did not outflow as expected.";

    spdlog::info("Final particle count: {}", linkedCellParticles->size());
    spdlog::info("SimpleOutflowRight test completed");
}


TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowLeft) {
    std::string in = "../input/test/outflow/simple_outflow_left.json";
    spdlog::info("Starting SimpleOutflowLeft test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // a single particle heading left (movement parallel to the y-axis)
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count before simulation.";
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0) << "Particle did not outflow as expected.";

    spdlog::info("Final particle count: {}", linkedCellParticles->size());
    spdlog::info("SimpleOutflowLeft test completed");
}


TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowFront) {
    std::string in = "../input/test/outflow/simple_outflow_front.json";
    spdlog::info("Starting SimpleOutflowFront test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count before simulation.";
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0) << "Particle did not outflow as expected.";

    spdlog::info("Final particle count: {}", linkedCellParticles->size());
    spdlog::info("SimpleOutflowFront test completed");
}


TEST_F(LinkedCellParticleContainerBehavioralTest, SimpleOutflowBack) {
    std::string in = "../input/test/outflow/simple_outflow_back.json";
    spdlog::info("Starting SimpleOutflowBack test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count before simulation.";
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 0) << "Particle did not outflow as expected.";

    spdlog::info("Final particle count: {}", linkedCellParticles->size());
    spdlog::info("SimpleOutflowBack test completed");
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
    spdlog::info("Starting ParticleReflectsAtTopBoundary test with input file: {}", in);
    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    // input has a single particle moving to the top like this and is very close to the boundary
    //            |
    //    *->     |
    //            |

    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count before simulation.";
    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count after simulation.";

    Particle particle = findFirstParticle(linkedCellParticles);

    // Check if it is reflected properly: Velocity's direction and magnitude will give it away.
    // The particle was moving along the y-Axis to go right initially, hence getV()[1]

    // Keeping the tolerance (kind of) large is important here as we're dealing with floating point numbers and a single particle's velocity
    // doesn't have to be accurate to the decimal point!
    EXPECT_NEAR(20.0, -particle.getV()[1], 2) << "Particle did not reflect at the top boundary as expected.";

    spdlog::info("ParticleReflectsAtTopBoundary test completed");
}


TEST_F(LinkedCellParticleContainerBehavioralTest, ParticleReflectsAtBottomBoundary) {
    std::string in = "../input/test/reflective/simple_reflective_bottom.json";
    spdlog::info("Starting ParticleReflectsAtBottomBoundary test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count after simulation.";

    Particle particle = findFirstParticle(linkedCellParticles);
    EXPECT_NEAR(-20.0, -particle.getV()[1], 2) << "Particle did not reflect at the bottom boundary as expected.";

    spdlog::info("ParticleReflectsAtBottomBoundary test completed");
}

// ParticleReflectsAtRightBoundary
TEST_F(LinkedCellParticleContainerBehavioralTest, ParticleReflectsAtRightBoundary) {
    std::string in = "../input/test/reflective/simple_reflective_right.json";
    spdlog::info("Starting ParticleReflectsAtRightBoundary test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count after simulation.";

    Particle particle = findFirstParticle(linkedCellParticles);

    EXPECT_NEAR(20.0, -particle.getV()[0], 1) << "Particle did not reflect at the right boundary as expected.";

    spdlog::info("ParticleReflectsAtRightBoundary test completed");
}

// ParticleReflectsAtLeftBoundary
TEST_F(LinkedCellParticleContainerBehavioralTest, ParticleReflectsAtLeftBoundary) {
    std::string in = "../input/test/reflective/simple_reflective_left.json";
    spdlog::info("Starting ParticleReflectsAtLeftBoundary test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count after simulation.";

    Particle particle = findFirstParticle(linkedCellParticles);

    // Compare initial velocity (-20.0) to the current one but in reversed direction
    EXPECT_NEAR(-20.0, -particle.getV()[0], 1) << "Particle did not reflect at the left boundary as expected.";

    spdlog::info("ParticleReflectsAtLeftBoundary test completed");
}

// ParticleReflectsAtFrontBoundary
TEST_F(LinkedCellParticleContainerBehavioralTest, ParticleReflectsAtFrontBoundary) {
    std::string in = "../input/test/reflective/simple_reflective_front.json";
    spdlog::info("Starting ParticleReflectsAtFrontBoundary test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count after simulation.";

    Particle particle = findFirstParticle(linkedCellParticles);
    EXPECT_NEAR(20.0, -particle.getV()[2], 1) << "Particle did not reflect at the front boundary as expected.";

    spdlog::info("ParticleReflectsAtFrontBoundary test completed");
}

// ParticleReflectsAtBackBoundary
TEST_F(LinkedCellParticleContainerBehavioralTest, ParticleReflectsAtBackBoundary) {
    std::string in = "../input/test/reflective/simple_reflective_back.json";
    spdlog::info("Starting ParticleReflectsAtBackBoundary test with input file: {}", in);

    Simulation simulation(in);

    auto particles = simulation.getParticles();
    auto linkedCellParticles = std::dynamic_pointer_cast<LinkedCellParticleContainer>(particles);
    ASSERT_NE(linkedCellParticles, nullptr);

    simulation.run();
    EXPECT_EQ(linkedCellParticles->size(), 1) << "Incorrect particle count after simulation.";

    Particle particle = findFirstParticle(linkedCellParticles);
    EXPECT_NEAR(-20.0, -particle.getV()[2], 1) << "Particle did not reflect at the back boundary as expected.";

    spdlog::info("ParticleReflectsAtBackBoundary test completed");
}
