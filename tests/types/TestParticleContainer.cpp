#include <gtest/gtest.h>

#include <random>

#include "particles/containers/directsum/DirectSumContainer.h"

/*
 * Test the default constructor of the ParticleContainer class.
 */
TEST(DirectSumContainer, DefaultConstructor) {
    DirectSumContainer container;
    EXPECT_EQ(container.capacity(), 0);
    EXPECT_EQ(container.size(), 0);
}

/*
 * Test the constructor with input (int) if reserve is called.
 */
TEST(DirectSumContainer, ConstructorWithReserve) {
    DirectSumContainer container(10);
    EXPECT_EQ(container.capacity(), 10);
    EXPECT_EQ(container.size(), 0);
}

/*
 * Test if particles are inserted correctly when passed by reference.
 */
TEST(DirectSumContainer, AddParticleCopy) {
    std::array<double, 3> v = {4, 5, 6};
    std::array<double, 3> x = {7, 8, 9};
    double m = 10;
    int type = 11;
    Particle p{x, v, m, type};
    DirectSumContainer container;
    container.addParticle(p);
    EXPECT_EQ(container.size(), 1);
    EXPECT_TRUE(container[0] == p);
}

/*
 * Test if particles are inserted correctly when passed by rvalue reference.
 */
TEST(DirectSumContainer, AddParticleMove) {
    std::array<double, 3> v = {4, 5, 6};
    std::array<double, 3> x = {7, 8, 9};
    double m = 10;
    int type = 11;
    Particle p{x, v, m, type};
    DirectSumContainer container;
    container.addParticle(std::move(p));
    EXPECT_EQ(container.size(), 1);
    EXPECT_TRUE(container[0] == p);
}

/*
 * Randomized test if many particles are inserted correctly (pass by reference).
 */
TEST(DirectSumContainer, ManyParticlesIteration) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1, 1);

    constexpr size_t N = 10000;
    DirectSumContainer container(N);
    std::array<Particle, N> particles_expected;

    for (size_t i = 0; i < N; i++) {
        std::array<double, 3> v = {dist(gen), dist(gen), dist(gen)};
        std::array<double, 3> x = {dist(gen), dist(gen), dist(gen)};
        double m = dist(gen);
        int type = i;
        particles_expected[i] = Particle{x, v, m, type};
        container.addParticle(particles_expected[i]);
    }

    for (size_t i = 0; i < N; i++) {
        EXPECT_TRUE(container[i] == particles_expected[i]);
    }
}