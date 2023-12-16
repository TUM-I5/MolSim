//
// Created by Feryal Ezgi on 16.12.2023.
//
#include <random>
#include "MaxwellBoltzmannDistribution.h"
std::array<double, 3> maxwellBoltzmannDistributedVelocity(double averageVelocity, size_t dimensions) {
    // we use a constant seed for repeatability.
    // random engine needs static lifetime otherwise it would be recreated for every call.
    static std::default_random_engine randomEngine(42);

    // when adding independent normally distributed values to all velocity components
    // the velocity change is maxwell boltzmann distributed
    std::normal_distribution<double> normalDistribution{0, 1};
    std::array<double, 3> randomVelocity{};
    for (size_t i = 0; i < dimensions; ++i) {
        randomVelocity[i] = averageVelocity * normalDistribution(randomEngine);
    }
    return randomVelocity;
}