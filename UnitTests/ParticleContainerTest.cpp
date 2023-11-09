//
// Created by markus on 11/7/23.
//
#include <gtest/gtest.h>
#include "../src/ParticleContainer.h"

TEST(ParticleContainerTest, DefaultConstructor) {
ParticleContainer container;
EXPECT_EQ(container.size(), 0);
}

TEST(ParticleContainerTest, ConstructorWithList) {
std::list<Particle> particlesList = {Particle(), Particle()};
ParticleContainer container(particlesList);
EXPECT_EQ(container.size(), particlesList.size());
}

TEST(ParticleContainerTest, AddParticle) {
ParticleContainer container;
Particle particle;
container.addParticle(particle);
EXPECT_EQ(container.size(), 1);
}