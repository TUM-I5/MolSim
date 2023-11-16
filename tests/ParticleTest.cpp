//
// Created by Feryal Ezgi on 5.11.2023.
//
#include <gtest/gtest.h>
#include "../src/models/ParticleContainer.h"
#include "spdlog/spdlog.h"


class ParticleTest : public testing::Test {
protected:
    Particle particle1;
    Particle particle2;

    void SetUp() override {
        particle1 = Particle(std::array<double, 3>{1.0, 2.0, 3.0}, std::array<double, 3>{0.1, 0.2, 0.3}, 2.0, 2);
        particle2 = Particle(std::array<double, 3>{2.0, 3.0, 0.0}, std::array<double, 3>{0.1, 0.2, 0.3}, 2.0, 2);
    }
};

// Test case to calculate the difference between particle coordinates
TEST_F(ParticleTest, TestDiffTo) {
    spdlog::info("Starting TestDiffTo");
    std::array<double, 3> diff = particle1.diffTo(particle2);

    EXPECT_EQ(diff[0], particle2.getX()[0] - particle1.getX()[0]);
    EXPECT_EQ(diff[1], particle2.getX()[1] - particle1.getX()[1]);
    EXPECT_EQ(diff[2], particle2.getX()[2] - particle1.getX()[2]);
    spdlog::info("TestDiffTo completed");

}

// Test case for distanceTo, by comparing it to the Euclidean distance formula
TEST_F(ParticleTest, TestDistanceTo) {
    spdlog::info("Starting TestDistanceTo");

    Particle particle1({1.0, 2.0, 3.0}, {0.0, 0.0, 0.0}, 1.0, 1);
    Particle particle2({1.0, 6.0, 3.0}, {0.0, 0.0, 0.0}, 1.0, 2);

    double distance = particle1.distanceTo(particle2);

    // Verify calculated distance, tolerate potential floating-point errors
    double expectedDistance = 4.0;  // Calculated using the Euclidean distance formula
    double tolerance = 1e-5;
    EXPECT_NEAR(distance, expectedDistance, tolerance) << "Distance between two particles is not as expected.";
    spdlog::info("TestDistanceTo completed");

}
