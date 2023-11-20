/**
 * MaxwellBoltzmannDistribution.h
 *
 * @date: 13.12.2019
 * @author: F. Gratl
 */

#pragma once

#include <array>
#include <random>

/**
 * Generate a random velocity vector according to the Maxwell-Boltzmann distribution, with a given average velocity.
 *
 * @param averageVelocity The average velocity of the brownian motion for the system.
 * @param dimensions Number of dimensions for which the velocity vector shall be generated. Set this to 2 or 3.
 * @return Array containing the generated velocity vector.
 */
std::array<double, 3> maxwellBoltzmannDistributedVelocity(double averageVelocity, size_t dimensions);
