#pragma once

#include "io/input/SimulationParams.h"
#include "io/input/xml/parser/configuration_pimpl.h"
#include "particles/ParticleContainer.h"

class XMLFileReader {
   public:
    SimulationParams readConfiguration(const std::string& filepath, ParticleContainer& particle_container);
};