/**
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <list>

#include "types/Particle.h"
#include "types/ParticleContainer.h"

/**
 * @brief Class to read particle data from a file
 *
 * The file format is as follows:
 * <pre>
 * \# Lines of comment start with '#' and are only allowed at the beginning of the file
 * \# Empty lines are not allowed.
 * \# The first line not being a comment has to be one integer, indicating the number of molecule data sets.
 * \# Molecule data consists of xyz-coordinates (3 double values), velocities (2 or 3 double values), mass (1 double value)
 * \#
 * \# xyz-coord      velocity        mass
 * 4
 * 0.0 0.0 0.0      0.0 0.0 0.0     1.0
 * 0.0 1.0 0.0     -1.0 0.0 0.0     3.0e-6
 * 0.0 5.36 0.0    -0.425 0.0 0.0   9.55e-4
 * 34.75 0.0 0.0    0.0 0.0296 0.0  1.0e-14
 * </pre>
 */
class FileReader {
   public:
    FileReader();
    virtual ~FileReader();

    /**
     * @brief Reads the file with the given path and fills the given ParticleContainer with the particle data stored in the file
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     */
    void readFile(const std::string& filepath, ParticleContainer& particle_container);
};
