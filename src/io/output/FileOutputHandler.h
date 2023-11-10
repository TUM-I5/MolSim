#pragma once

#include <string>

#include "VTKWriter.h"
#include "XYZWriter.h"
#include "types/ParticleContainer.h"

/**
 * @brief Wrapper class to abstract the writing of output files
 *
 * Currently only supports writing to VTK files, but could be extended to support other file formats.
 */
class FileOutputHandler {
   public:
    enum class OutputFormat { VTK,
                              XYZ,
                              NONE };

   private:
    OutputFormat output_format;
    std::string output_dir_path;

   public:
    /**
     * @brief Construct a new FileOutputHandler object
     *
     * @param output_format The format of the output files
     */
    FileOutputHandler(OutputFormat output_format, const std::string& output_dir_path) : output_format(output_format), output_dir_path(output_dir_path) {}

    /**
     * @brief Writes the given ParticleContainer to a file
     *
     * @param output_dir_path The path to the directory in which to save the output
     * @param output_file_name_base The base name of the output files
     * @param iteration The current iteration of the simulation
     * @param particle_container The ParticleContainer to write to the file
     *
     * Currently only supports writing to VTK files.
     * The output files will be named '<output_file_name_base>_<iteration>.vtk', such that the files can be grouped by paraview for visualization.
     */
    void writeFile(const std::string& output_file_name_base, int iteration, const ParticleContainer& particle_container);

   private:
    void writeVTKFile(const std::string& output_dir_path, const std::string& output_file_name_base, int iteration, const ParticleContainer& particle_container);
    void writeXYZFile(const std::string& output_dir_path, const std::string& output_file_name_base, int iteration, const ParticleContainer& particle_container);
};