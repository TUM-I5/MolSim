/*
 * MaxwellBoltzmannDistribution.h
 *
 * @Date: 13.12.2019
 * @Author: F. Gratl
 */

#pragma once

#include <random>
#include <array>

/**
 * Generate a random velocity vector according to the Maxwell-Boltzmann distribution, with a given average velocity.
 *
 * @param averageVelocity The average velocity of the brownian motion for the system.
 * @param dimensions Number of dimensions for which the velocity vector shall be generated. Set this to 2 or 3.
 * @return Array containing the generated velocity vector.
 */
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
