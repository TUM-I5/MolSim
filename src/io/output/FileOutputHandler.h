#pragma once

#include <memory>
#include <string>

#include "io/output/vtk/VTKWriter.h"
#include "io/output/xyz/XYZWriter.h"
#include "particles/containers/ParticleContainer.h"

/**
 * @brief Wrapper class to abstract the writing of output files
 *
 * Currently there are two supported output formats: VTK and XYZ. Additionally a 'NONE' format is available, which does not write any
 * output.
 */
class FileOutputHandler {
   public:
    /**
     * @brief Enum class to specify the output format
     */
    enum class OutputFormat { VTK, XYZ, NONE };

   private:
    /**
     * @brief Saves the output format given
     */
    const OutputFormat output_format;

    /**
     * @brief Path to the directory in which to save the output
     */
    const std::string output_dir_path;

    /**
     * @brief Pointer to the FileWriter object to use
     */
    std::unique_ptr<FileWriter> file_writer;

   public:
    /**
     * @brief Construct a new FileOutputHandler object
     *
     * @param output_format The format of the output files
     * @param output_dir_path The path to the directory in which to save the output;
     */
    explicit FileOutputHandler(OutputFormat output_format, const std::string& output_dir_path = "./output");

    /**
     * @brief Writes the given ParticleContainers particle data to a file
     *
     * @param iteration The current iteration number of the simulation
     * @param particle_container The ParticleContainer to write to the file
     */
    void writeFile(int iteration, const std::unique_ptr<ParticleContainer>& particle_container) const;
};