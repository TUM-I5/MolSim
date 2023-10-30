/**
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <list>

#include "Particle.h"
#include "ParticleContainer.h"

class FileReader {
   public:
    FileReader();
    virtual ~FileReader();

    /**
     * @brief Reads the file with the given path and fills the given ParticleContainer with the particle data stored in the file
     * @param particle_container ParticleContainer to be filled
     * @param filepath Path to the file to be read
     */
    void readFile(ParticleContainer& particle_container, std::string& filepath);
};
