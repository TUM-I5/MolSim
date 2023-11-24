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
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     *
     * Reads the .ps file with the given path and fills the given ParticleContainer with the particle data stored in the file.
     * For more information about the file format, see \ref InputFileFormats "Input File Formats".
     */
    SimulationParams readFile(const std::string& filepath, std::unique_ptr<ParticleContainer>& particle_container) const override;
};
