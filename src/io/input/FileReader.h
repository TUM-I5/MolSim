#pragma once

#include <string>

#include "io/input/SimulationParams.h"
#include "particles/containers/ParticleContainer.h"

/**
 * @brief Abstract base class for all custom file readers
 */
class FileReader {
   public:
    /**
     * @brief virtual destructor for correct cleanup of derived classes
     */
    virtual ~FileReader() = default;

    /**
     * @brief Reads the file with the given path and fills the given ParticleContainer with the particle data stored in the file
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     */
    virtual SimulationParams readFile(const std::string& filepath, std::unique_ptr<ParticleContainer>& particle_container) const = 0;
};

class FileFormatException : public std::exception {};
