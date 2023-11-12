//
// Created by Feryal Ezgi on 5.11.2023.
//

#include <gtest/gtest.h>

#include "../src/models/ParticleContainer.h"
#include "../src/models/Particle.h"

TEST(ParticleTest, TestDiffTo) {
    Particle particle1(std::array<double, 3>{1.0, 2.0, 3.0}, std::array<double, 3>{0.1, 0.2, 0.3}, 2.0, 2);
    Particle particle2(std::array<double, 3>{2.0, 3.0, 0.0}, std::array<double, 3>{0.1, 0.2, 0.3}, 2.0, 2);
    std::array<double, 3> diff = particle1.diffTo(particle2);

    EXPECT_EQ(diff[0], particle2.getX()[0] - particle1.getX()[0]);
    EXPECT_EQ(diff[1], particle2.getX()[1] - particle1.getX()[1]);
    EXPECT_EQ(diff[2], particle2.getX()[2] - particle1.getX()[2]);
}


TEST(ParticleTest, TestDistanceTo) {
    Particle particle1(std::array<double, 3>{1.0, 2.0, 3.0}, std::array<double, 3>{0.1, 0.2, 0.3}, 2.0, 2);
    Particle particle2(std::array<double, 3>{2.0, 3.0, 0.0}, std::array<double, 3>{0.1, 0.2, 0.3}, 2.0, 2);
    std::array<double, 3> diff = particle1.diffTo(particle2);

    EXPECT_EQ(diff[0], particle2.getX()[0] - particle1.getX()[0]);
    EXPECT_EQ(diff[1], particle2.getX()[1] - particle1.getX()[1]);
    EXPECT_EQ(diff[2], particle2.getX()[2] - particle1.getX()[2]);
}


