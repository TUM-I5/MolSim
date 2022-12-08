/*
 * ParticleGenerator.h
 *
 *  Created on: 04.12.2022
 *      Author: wohlrapp
 */

#pragma once

#include "../model/ParticleContainer.h"
#include "../model/Cuboid.h"
#include "../model/Sphere.h"
#include "./MaxwellBoltzmannDistribution.h"
#include "./ArrayUtils.h"

#include <iostream>

namespace ParticleGenerator
{

    /**
     * @brief generates all particles of a cuboid and adds them to the particle container
     * @param particleContainer contains all particles for the simulation
     * @param cuboid contains all necessary parameters for the construction of the cuboid
     */
    inline void generateCuboid(ParticleContainer &particleContainer, Cuboid &cuboid)
    {
        // Dimension
        int dimension = 2;

        // Variable init
        std::array<double, 3> lowerLeftCorner = cuboid.getX();
        std::array<double, 3> initV = cuboid.getV();
        double m = cuboid.getM();
        int type = cuboid.getType(); 

        std::array<int, 3> n = cuboid.getN();
        int numParticles = n[0] * n[1] * n[2];

        double meshWidth = cuboid.getH();
        double meanV = cuboid.getMeanV();

        // reserve memory for particles
        particleContainer.reserveMemoryForParticles(numParticles);

        // create particles
        std::array<double, 3> position;
        std::array<double, 3> velocity;
        std::array<double, 3> maxwellBoltzmann;

        for (int x = 0; x < n[0]; x++)
        {
            for (int y = 0; y < n[1]; y++)
            {
                for (int z = 0; z < n[2]; z++)
                {
                    // initialize brownian motion
                    maxwellBoltzmann = maxwellBoltzmannDistributedVelocity(meanV, dimension);
                    velocity = initV + maxwellBoltzmann;

                    position[0] = lowerLeftCorner[0] + (x * meshWidth);
                    position[1] = lowerLeftCorner[1] + (y * meshWidth);
                    position[2] = lowerLeftCorner[2] + (z * meshWidth);
                    particleContainer.addParticle(position, velocity, m, type);
                }
            }
        }
    }

    /**
     * @brief generates all particles of a sphere and adds them to the particle container, if radius is 0, there is still
     * @param particleContainer contains all particles for the simulation
     * @param sphere contains all necessary parameters for the construction of the sphere
     */
    inline void generateSphere(ParticleContainer &particleContainer, Sphere &sphere)
    {
        // Dimension
        int dimension = 2;

        std::array<double, 3> center = sphere.getCenter();
        double m = sphere.getM();
        int r = sphere.getR();
        double meshWidth = sphere.getH();
        double meanV = sphere.getMeanV();
        std::array<double, 3> initV = sphere.getV();
        int type = sphere.getType(); 

        // number of particles which are later allocated
        int numParticles = 0;

        // create particles
        std::array<double, 3> position;
        std::array<double, 3> velocity = {0, 0, 0};
        std::array<double, 3> maxwellBoltzmann;

        // Variable init
        std::array<double, 3> startingPoint =
            {center[0] - (r - 1) * meshWidth - 0.5 * meshWidth, center[1] - (r - 1) * meshWidth - 0.5 * meshWidth, center[2] - ((dimension - 2) * ((r - 1) * meshWidth - 0.5 * meshWidth))};

        // first we need to iterate the loop once to count the number of particles
        for (int x = 0; x < 2 * r; x++)
        {
            for (int y = 0; y < 2 * r; y++)
            {
                position[0] = startingPoint[0] + (x * meshWidth);
                position[1] = startingPoint[1] + (y * meshWidth);
                position[2] = startingPoint[2];
                if (ArrayUtils::L2Norm(position - center) <= r * meshWidth)
                {
                    numParticles++;
                }

                // if three dimensions, we need to do this for every z
                for (int z = 1; z < (dimension - 2) * 2 * r; z++)
                {
                    position[0] = startingPoint[0] + (x * meshWidth);
                    position[1] = startingPoint[1] + (y * meshWidth);
                    position[2] = startingPoint[2] + (z * meshWidth);
                    if (ArrayUtils::L2Norm(position - center) <= r * meshWidth)
                    {
                        numParticles++;
                    }
                }
            }
        }

        // reserve memory for particles
        particleContainer.reserveMemoryForParticles(numParticles);

        // iterate again to actually add the particles to the container
        for (int x = 0; x < 2 * r; x++)
        {
            for (int y = 0; y < 2 * r; y++)
            {
                // do this once with z zero if we only have two dimensions
                // initialize brownian motion
                maxwellBoltzmann = maxwellBoltzmannDistributedVelocity(meanV, dimension);
                velocity = initV + maxwellBoltzmann;

                position[0] = startingPoint[0] + (x * meshWidth);
                position[1] = startingPoint[1] + (y * meshWidth);
                position[2] = startingPoint[2];

                // normally r-0.5 * mesh width but we want to include a bit more particles
                if (ArrayUtils::L2Norm(position - center) <= r * meshWidth)
                {
                    particleContainer.addParticle(position, velocity, m, type);
                }

                // if three dimensions, we need to do this for every z
                for (int z = 1; z < (dimension - 2) * 2 * r; z++)
                {

                    // initialize brownian motion
                    maxwellBoltzmann = maxwellBoltzmannDistributedVelocity(meanV, dimension);
                    velocity = initV + maxwellBoltzmann;

                    position[0] = startingPoint[0] + (x * meshWidth);
                    position[1] = startingPoint[1] + (y * meshWidth);
                    position[2] = startingPoint[2] + (z * meshWidth);
                    if (ArrayUtils::L2Norm(position - center) <= r * meshWidth)
                    {
                        particleContainer.addParticle(position, velocity, m), type;
                    }
                }
            }
        }
    }
}