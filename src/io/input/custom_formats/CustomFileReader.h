#pragma once

#include <string>

#include "particles/ParticleContainer.h"

/**
 * @brief Abstract base class for all file readers
 */
class CustomFileReader {
   public:
    virtual ~CustomFileReader() = default;

    /**
     * @brief Reads the file with the given path and fills the given ParticleContainer with the particle data stored in the file
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     */
    virtual void readFile(const std::string& filepath, ParticleContainer& particle_container) const = 0;

    class FileFormatException : public std::exception {};
};