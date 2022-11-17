/*
 * CuboidInputReader.h
 *
 *  Created on: 16.11.2022
 *      Author: borisov
 */

#pragma once

#include "./InputReader.h"
#include "../model/Cuboid.h"
#include "../model/ParticleContainer.h"

/**
 * @brief Class that reads a Cuboid from a file and then creates Particles representing that cuboid
 */
class CuboidInputReader : public InputReader {
public:
    /**
     * @brief Reads a file containing cuboid data and calls the method createParticles
     * 
     * @param particleContainer The container in which the createParticles method will create the particles
     * @param filename The pointer to the filename of the input file
     */
    virtual void readInput(ParticleContainer& particleContainer, const char *filename);

    /**
     * @brief Creates particles representing a cuboid
     * 
     * @param particleContainer The container in which the created particles will be stored
     * @param cuboid The cuboid for which the particles will be created 
     */
    void createParticles(ParticleContainer& particleContainer, Cuboid& cuboid);
};
