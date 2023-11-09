#pragma once

#include <string>

#include "types/ParticleContainer.h"

/**
 * @brief Abstract base class for all file readers
 */
class FileReader {
   public:
    virtual ~FileReader() = default;
    /**
     * @brief Reads the file with the given path and fills the given ParticleContainer with the particle data stored in the file
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     */
    virtual void readFile(const std::string& filepath, ParticleContainer& particle_container) = 0;
};