#pragma once

#include <list>

#include "io/input/FileReader.h"
#include "particles/Particle.h"
#include "particles/containers/ParticleContainer.h"

/**
 * @brief Class to read particle data from a '.ps' file
 */
class PsFileReader : public FileReader {
   public:
    /**
     * @brief Reads the file with the given path and fills the given ParticleContainer with the particle data stored in the file
     * (see \ref InputFileFormats "Input File Formats" for details on the .ps file format).
     *
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     * @return Default SimulationParams object (.ps only contains particle data, but no simulation parameters). Necessary for the
     * FileReader interface.
     */
    SimulationParams readFile(const std::string& filepath, std::unique_ptr<ParticleContainer>& particle_container) const override;
};
