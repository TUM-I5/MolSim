#pragma once

#include <optional>
#include <set>

#include "io/input/custom_formats/cub/CubFileReader.h"
#include "io/input/custom_formats/ps/PsFileReader.h"
#include "io/input/xml/XMLFileReader.h"
#include "simulation/SimulationParams.h"

/**
 * @brief Wrapper class to abstract the reading of input files
 *
 * This class abstracts the reading and writing of files, so that the Simulation class does not have to know about the concrete
 * implementations. Automatically determines correct file format using the file extension.
 */
class FileInputHandler {
   public:
    /**
     * @brief Reads the input file and stores the particles in the given ParticleContainer. Other simulation parameters are returned as
     * SimulationParams object.
     *
     * @param input_file_path The path to the input file
     * @param particle_container The ParticleContainer to store the particles in#
     * @return SimulationParams containing the simulation parameters
     *
     * Reads the input file and generate and stores particles in the given `ParticleContainer`.
     * Other parameters are stored in a `SimulationParams` object and returned.
     * For more information about the output file formats, see \ref InputFileFormats "Input File Formats"
     */
    static SimulationParams readFile(const std::string& input_file_path, std::unique_ptr<ParticleContainer>& particle_container);

    /**
     * @brief Returns a list of supported input file extensions
     *
     * @return std::vector<std::string> List of supported input file extensions
     *
     * Returns a list of supported input file extensions.
     * Supported file formats are:
     * - .ps
     * - .cub
     * - .xml
     * For more information about the output file formats, see \ref InputFileFormats "Input File Formats"
     */
    static std::set<std::string> get_supported_input_file_extensions();
};
