#pragma once

#include <memory>

#include "io/input/FileReader.h"
#include "io/input/SimulationParams.h"
#include "io/input/xml/simulation_schema.h"
#include "particles/containers/ParticleContainer.h"

/**
 * @brief Class to read particle data from a '.xml' file
 */
class XMLFileReader : public FileReader {
   public:
    SimulationParams readFile(const std::string& filepath, std::unique_ptr<ParticleContainer>& particle_container) const override;
};