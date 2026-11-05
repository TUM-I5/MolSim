/*
 * MaxwellBoltzmannDistribution.h
 *
 * @Date: 13.12.2019
 * @Author: F. Gratl
 */

#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <random>
#include <stdexcept>

/**
 * Generate a random velocity vector according to the Maxwell-Boltzmann distribution, with a given
 * average velocity.
 *
 * @param averageVelocity The average velocity of the brownian motion for the system.
 * @param dimensions Number of dimensions for which the velocity vector shall be generated. Set this
 * to 2 or 3.
 * @return Array containing the generated velocity vector.
 */
std::array<double, 3> maxwellBoltzmannDistributedVelocity(double averageVelocity,
                                                          size_t dimensions) {
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

// TODO make more phisically accurate (temperataure should take the average velocety into account -
// accurate total energy)

/**
 * Generate a random velocity vector according to the Maxwell–Boltzmann distribution.
 *
 * Each velocity component is sampled from a normal distribution with mean zero and
 * standard deviation sqrt(k_B * temperatureKelvin / particleMassKg), where k_B is
 * the Boltzmann constant.
 *
 * @param temperatureKelvin The temperature in kelvin. Must be positive.
 * @param particleMassKg The particle mass in kilograms. Must be positive.
 * @param dimensions Number of dimensions for which the velocity vector shall be generated. Set this
 * to 2 or 3.
 * @return Array containing the generated velocity vector.
 */
std::array<double, 3> maxwellBoltzmannDistributedVelocity(double temperatureKelvin,
                                                          double particleMassKg,
                                                          std::size_t dimensions) {
    if (temperatureKelvin < 0.0 || !std::isfinite(temperatureKelvin)) {
        throw std::invalid_argument("Temperature must be finite and non-negative");
    }
    if (particleMassKg <= 0.0 || !std::isfinite(particleMassKg)) {
        throw std::invalid_argument("Particle mass must be finite and positive");
    }

    constexpr double boltzmannConstant = 1.380649e-23;  // J/K
    const double componentStdDev =
        std::sqrt(boltzmannConstant * temperatureKelvin / particleMassKg);

    static std::default_random_engine randomEngine(42);
    std::normal_distribution<double> distribution(0.0, componentStdDev);

    std::array<double, 3> randomVelocity{};
    for (std::size_t i = 0; i < dimensions; ++i) {
        randomVelocity[i] = distribution(randomEngine);
    }

    return randomVelocity;
}