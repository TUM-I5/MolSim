//
// Created by alex on 06.11.2022.
//
#include <gtest/gtest.h>

#include "data/ParticleContainer.h"
#include "data/Particle.h"

#include <vector>

TEST(ParticleContainer, ParticleContainer_Empty) {
    ParticleContainer pc{};
    EXPECT_EQ(pc.size(), 0);
}

/**
 * Check if the container actually stores all particles and does not mutate them.
 * */
TEST(ParticleContainer, ParticleContainer_Count) {
    std::vector<Particle> vec{Particle{0}, Particle{1}, Particle{2}, Particle{3}};
    ASSERT_EQ(vec.size(), 4);

    ParticleContainer pc{vec};
    EXPECT_EQ(pc.size(), 4) << "Particle container does not store the right amount of items.";

    for (unsigned long i{ 0 }; i < pc.size(); i++) {
        for (unsigned long j { 0 }; j < vec.size(); j++) {
            if (pc.getParticle(i) == vec[j]) goto success;
        } // inner
        FAIL() << "Unknown element in ParticleContainer" << " Index i: " << i;
    success: continue; // continue with next particle
    } // outer
}
