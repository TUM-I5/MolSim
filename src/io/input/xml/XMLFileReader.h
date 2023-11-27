#pragma once

#include <memory>

#include "io/input/FileReader.h"
#include "io/input/xml/parser/simulation_schema.h"
#include "particles/ParticleContainer.h"
#include "simulation/SimulationParams.h"

/**
 * @brief Class to read particle data from a '.xml' file
 */
class XMLFileReader : public FileReader {
   public:
    /**
     * @brief Reads the file with the given path and fills the given ParticleContainer with the particle data stored in the file
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     */
    SimulationParams readFile(const std::string& filepath, ParticleContainer& particle_container) const override;
};