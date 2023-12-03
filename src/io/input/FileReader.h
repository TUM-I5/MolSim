#pragma once

#include <string>

#include "particles/containers/ParticleContainer.h"
#include "simulation/SimulationParams.h"

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
     * @return SimulationParams object containing the simulation parameters given in the file. Unspecified parameters are set to a default
     * value.
     */
    virtual SimulationParams readFile(const std::string& filepath, std::unique_ptr<ParticleContainer>& particle_container) const = 0;

    /**
     * @brief Exception to be thrown when the file format is invalid
     */
    class FileFormatException : public std::exception {
       public:
        explicit FileFormatException(std::string message) : message_(std::move(message)) {}

        [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); }

       private:
        std::string message_;
    };
};
