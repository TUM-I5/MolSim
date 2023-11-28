

#pragma once

#include <list>

#include "io/output/FileWriter.h"
#include "io/output/vtk/parser/vtk-unstructured.h"
#include "particles/Particle.h"

/**
 * @brief Class to write particle data to a vtk file
 */
class VTKWriter : public FileWriter {
   public:
    /**
     * @brief Writes the data of the given ParticleContainer to a vtk file
     * @param output_dir_path Path to the directory in which to save the output
     * @param iteration The current iteration
     * @param particle_container ParticleContainer to be used
     */
    void writeFile(const std::string& output_dir_path, int iteration, const ParticleContainer& particle_container) const override;

   private:
    /**
     * Creates a VTKFile_t object with the given number of particles.
     */
    VTKFile_t initializeOutput(int numParticles) const;

    /**
     * Writes a given particle to the given VTKFile_t object.
     */
    void plotParticle(VTKFile_t& file, const Particle& p) const;
};
