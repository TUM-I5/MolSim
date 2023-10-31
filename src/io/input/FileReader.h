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

class FileReader {
   public:
    FileReader();
    virtual ~FileReader();

    /**
     * @brief Reads the file with the given path and fills the given ParticleContainer with the particle data stored in the file
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     */
    void readFile(std::string& filepath, ParticleContainer& particle_container);
};
