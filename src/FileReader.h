/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <string>

#include "ParticleContainer.h"

/**
 * @brief Reader to read initial particle data from file
 *
 * Format:
 *  - Leading lines starting with '#' are ignored (comments)
 *  - First real line is the number of particles N
 *  - N lines for one particle each. `x1 x2 x3  v1 v2 v3  m`
 *
 * Example:
 * @code
 * # Sun, Earth, Jupiter, Halley
 * 4
 * 0.0 0.0 0.0      0.0 0.0 0.0     1.0
 * 0.0 1.0 0.0     -1.0 0.0 0.0     3.0e-6
 * 0.0 5.36 0.0    -0.425 0.0 0.0   9.55e-4
 * 34.75 0.0 0.0    0.0 0.0296 0.0  1.0e-14
 * @endcode
 */
class FileReader {
   public:
    FileReader();
    virtual ~FileReader();

    /**
     * @brief Reads a list of initial particle data from @p filename and populates @p particles
     * with them
     *
     * Exits the program on a malformed file or cant be opened
     *
     * @param particles Container to fill with particle data
     * @param filename Path to the input file
     */
    static void ReadFile(ParticleContainer &particles, const std::string &filename);
};
