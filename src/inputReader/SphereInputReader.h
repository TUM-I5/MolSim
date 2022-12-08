/*
 * CuboidInputReader.h
 *
 *  Created on: 16.11.2022
 *      Author: borisov
 */

#pragma once

#include "./InputReader.h"
#include "../model/Sphere.h"

/**
 * @brief Class that reads a Sphere from a file and then creates Particles representing that cuboid
 */
class SphereInputReader : public InputReader
{
public:
    /**
     * @brief Reads a file containing sphere data and calls the method createParticles
     * @param programParameters The reference to a wrapper for the programParameters, when reading the input, new particles can be added to the container inside
     * @param filename The pointer to the filename of the input file
     */
    virtual void readInput(ProgramParameters &programParameters, const char *filename);
};
