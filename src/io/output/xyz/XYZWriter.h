#pragma once

#include <fstream>
#include <list>

#include "io/output/FileWriter.h"
#include "particles/Particle.h"
#include "particles/containers/ParticleContainer.h"

/**
 * @brief Class to write particle data to a xyz file
 */
class XYZWriter : public FileWriter {
   public:
    /**
     * @brief Writes the data of the given ParticleContainer to a xyz file
     * @param output_dir_path Path to the directory in which to save the output
     * @param iteration The current iteration
     * @param particle_container ParticleContainer to be used
     */
    void writeFile(const std::string& output_dir_path, int iteration,
                   const std::unique_ptr<ParticleContainer>& particle_container) const override;
};
