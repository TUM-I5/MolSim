#pragma once

#include <memory>

#include "io/input/SimulationParams.h"
#include "io/input/custom_formats/CustomFileReader.h"
#include "io/input/xml/simulation_schema.h"
#include "particles/ParticleContainer.h"

/**
 * @brief Class to read particle data from a '.xml' file
 */
class XMLFileReader {
   public:
    SimulationParams readConfiguration(const std::string& filepath, ParticleContainer& particle_container) const;
};