#pragma once

#include <string>

#include "ParticleContainer.h"

/**
 * @brief Loads a JSON simulation configuration and initializes particles.
 */
class FileReader {
   public:
    /**
     * @brief Reads a JSON configuration and adds its cuboid particles to a container.
     * @param particles Container to populate.
     * @param filename Path to the JSON configuration file.
     * @throws std::runtime_error If the file cannot be opened or the configuration is invalid.
     * @throws nlohmann::json::exception If the JSON is malformed or has invalid field types.
     */
    static void ReadFile(ParticleContainer &particles, const std::string &filename);
};
