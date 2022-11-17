/*
 * CuboidInputReader.cpp
 *
 *  Created on: 16.11.2022
 *      Author: borisov
 */

#include "./CuboidInputReader.h"
#include "../utils/MaxwellBoltzmannDistribution.h"
#include "../utils/ArrayUtils.h"

void CuboidInputReader::readInput(ParticleContainer& particleContainer, Cuboid& cuboid) {
    // Dimension 
    int dimension = 2;

    // Variable init
    std::array<double, 3> lowerLeftCorner = cuboid.getX();
    std::array<double, 3> initV = cuboid.getV();
    double m = cuboid.getM();
    
    std::array<int, 3> n = cuboid.getN();
    int numParticles = n[0] * n[1] * n[2];

    double meshWidth = cuboid.getH();

    // reserve memory for particles 
    particleContainer.reserveMemoryForParticles(numParticles);

    // initialize brownian motion
    std::array<double, 3> maxwellBoltzmann = maxwellBoltzmannDistributedVelocity(cuboid.getMeanV(), dimension);
    initV[0] += maxwellBoltzmann[0];
    initV[1] += maxwellBoltzmann[1];
    initV[2] += maxwellBoltzmann[2];

    // create particles
    std::array<double, 3> position;
    for (int x = 0; x < n[0]; x++) {
        for (int y = 0; y < n[1]; y++) {
            for (int z = 0; z < n[2]; z++) {
                position[0] = lowerLeftCorner[0] + (x * meshWidth);
                position[1] = lowerLeftCorner[1] + (y * meshWidth);
                position[2] = lowerLeftCorner[2] + (z * meshWidth);
                particleContainer.addParticle(position, initV, m);
            }
        }
    }
}