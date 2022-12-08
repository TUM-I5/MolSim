/*
 * CuboidInputReader.h
 *
 *  Created on: 16.11.2022
 *      Author: borisov
 */

#pragma once

#include "./InputReader.h"
#include "../model/Sphere.h"
#include "../model/ParticleContainer.h"

/**
 * @brief Class that reads a Sphere from a file and then creates Particles representing that cuboid
 */
class SphereInputReader : public InputReader
{
public:
    /**
     * @brief Reads a file containing sphere data and calls the method createParticles
     *
     * @param particleContainer The container in which the createParticles method will create the particles
     * @param filename The pointer to the filename of the input file
     */
    virtual void readInput(ParticleContainer &particleContainer, const char *filename);

    /**
     * @brief Creates particles representing a sphere
     *
     * @param particleContainer The container in which the created particles will be stored
     * @param cuboid The cuboid for which the particles will be created
     */
    void createParticles(ParticleContainer &particleContainer, Sphere &spere);
};
