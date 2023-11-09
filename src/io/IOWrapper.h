#pragma once

#include "io/input/CubFileReader.h"
#include "io/input/PsFileReader.h"
#include "io/output/VTKWriter.h"

/**
 * @brief IO wrapper class to abstract reading and writing of files
 *
 * This class abstracts the reading and writing of files, so that the Simulation class does not have to know about the concrete implementations
 * Currently uses hard coded PsFileReader and VTKWriter
 */
class IOWrapper {
    PsFileReader txt_file_reader;
    CubFileReader cub_file_reader;
    VTKWriter vtk_writer;

   public:
    /**
     * @brief Reads the input file and stores the particles in the given ParticleContainer
     *
     * @param input_file_path The path to the input file
     * @param particle_container The ParticleContainer to store the particles in
     *
     * Currently only supports files with the following format:
     * # Lines of comment start with '#' and are only allowed at the beginning of the file
     * # Empty lines are not allowed.
     * # The first line not being a comment has to be one integer, indicating the number of
     * # molecule data sets.
     * #
     * # Molecule data consists of
     * # * xyz-coordinates (3 double values)
     * # * velocities (2 or 3 double values)
     * # * mass (1 double value)
     * #
     * # xyz-coord      velocity        mass
     * 4
     * 0.0 0.0 0.0      0.0 0.0 0.0     1.0
     * 0.0 1.0 0.0     -1.0 0.0 0.0     3.0e-6
     * 0.0 5.36 0.0    -0.425 0.0 0.0   9.55e-4
     * 34.75 0.0 0.0    0.0 0.0296 0.0  1.0e-14
     */
    void readFile(const std::string& input_file_path, ParticleContainer& particle_container);

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
    void writeFile(const std::string& output_dir_path, const std::string& output_file_name_base, int iteration, const ParticleContainer& particle_container);
};