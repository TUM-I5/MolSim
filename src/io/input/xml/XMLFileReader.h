#pragma once

#include "io/input/SimulationParams.h"
#include "io/input/custom_formats/CustomFileReader.h"
#include "io/input/xml/parser/simulation_schema-pimpl.h"
#include "particles/ParticleContainer.h"

/**
 * @brief Class to read particle data from a '.xml' file
 */
class XMLFileReader {
   public:
    SimulationParams readConfiguration(const std::string& filepath, ParticleContainer& particle_container) const;
};