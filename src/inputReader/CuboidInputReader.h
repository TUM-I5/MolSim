/*
 * CuboidInputReader.h
 *
 *  Created on: 16.11.2022
 *      Author: borisov
 */

#pragma once

#include "./InputReader.h"
#include "../model/Cuboid.h"
#include "../model/ProgramParameters.h"

/**
 * @brief Class that reads a Cuboid from a file and then creates Particles representing that cuboid
 */
class CuboidInputReader : public InputReader
{
public:
    /**
     * @brief Reads a file containing cuboid data and calls the method createParticles
     *
     * @param programParameters The reference to a wrapper for the programParameters, when reading the input, new particles can be added to the container inside
     * @param filename The pointer to the filename of the input file
     */
    virtual void readInput(ProgramParameters &programParameters, const char *filename);
};
