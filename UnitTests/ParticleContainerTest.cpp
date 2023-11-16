//
// Created by markus on 11/7/23.
//
#include <gtest/gtest.h>
#include "../src/ParticleContainer.h"

// testing the default constructor of the ParticleContainer
TEST(ParticleContainerTest, DefaultConstructor) {
ParticleContainer container;
EXPECT_EQ(container.size(), 0);
}

// testing the constructor with a list as a parameter
TEST(ParticleContainerTest, ConstructorWithList) {
std::vector<Particle> particlesList = {Particle(), Particle()};
ParticleContainer container((particlesList));
EXPECT_EQ(container.size(), particlesList.size());
}

// testing the adding function of the ParticleContainer
TEST(ParticleContainerTest, AddParticle) {
ParticleContainer container;
Particle particle;
container.addParticle(particle);
EXPECT_EQ(container.size(), 1);
}