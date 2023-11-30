

#pragma once

#include <list>

#include "io/output/FileWriter.h"
#include "io/output/vtk/parser/vtk-unstructured.h"
#include "particles/Particle.h"

/**
 * @brief Class to write particle data to a .vtk file
 */
class VTKWriter : public FileWriter {
   public:
    /**
     * @brief Writes the data of the given ParticleContainer to a .vtk file
     *
     * @param output_dir_path Path to the directory in which to save the output file
     * @param iteration The current iteration number
     * @param particle_container ParticleContainer whose particles are to be written
     */
    void writeFile(const std::string& output_dir_path, int iteration,
                   const std::unique_ptr<ParticleContainer>& particle_container) const override;

   private:
    /**
     * @brief Creates a VTKFile_t object with the given number of particles.
     *
     * @param numParticles Number of particles to be plotted
     * @return VTKFile_t object with the given number of particles
     */
    [[nodiscard]] static VTKFile_t initializeOutput(int numParticles);

    /**
     * @brief Writes a given particle to the given VTKFile_t object.
     *
     * @param file VTKFile_t object to write to
     * @param p Particle to be written
     */
    static void plotParticle(VTKFile_t& file, const Particle& p);
};
