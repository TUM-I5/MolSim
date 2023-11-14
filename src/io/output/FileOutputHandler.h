#pragma once

#include <string>

#include "VTKWriter.h"
#include "XYZWriter.h"
#include "particles/ParticleContainer.h"

/**
 * @brief Wrapper class to abstract the writing of output files
 *
 * Currently there are two supported output formats: VTK and XYZ. Additionally a 'NONE' format is available, which does not write any output.
 */
class FileOutputHandler {
   public:
    enum class OutputFormat { VTK, XYZ, NONE };

   private:
    const OutputFormat output_format;
    const std::string output_dir_path;

   public:
    /**
     * @brief Construct a new FileOutputHandler object
     *
     * @param output_format The format of the output files
     * @param output_dir_path The path to the directory in which to save the output
     */
    FileOutputHandler(const OutputFormat output_format, const std::string& output_dir_path = "./output");

    /**
     * @brief Writes the given ParticleContainer to a file
     *
     * @param iteration The current iteration of the simulation
     * @param particle_container The ParticleContainer to write to the file
     */
    void writeFile(int iteration, const ParticleContainer& particle_container) const;

   private:
    /**
     * @brief Writes the given ParticleContainer to a VTK file
     *
     * @param output_dir_path The path to the directory in which to save the output
     * @param iteration The current iteration of the simulation
     * @param particle_container The ParticleContainer to write to the file
     */
    void writeVTKFile(const std::string& output_dir_path, int iteration, const ParticleContainer& particle_container) const;

    /**
     * @brief Writes the given ParticleContainer to a XYZ file
     *
     * @param output_dir_path The path to the directory in which to save the output
     * @param iteration The current iteration of the simulation
     * @param particle_container The ParticleContainer to write to the file
     */
    void writeXYZFile(const std::string& output_dir_path, int iteration, const ParticleContainer& particle_container) const;
};