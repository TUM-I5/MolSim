#pragma once

#include "FileReader.h"

class CubFileReader : public FileReader {
   public:
    /**
     * @brief Reads the '.cub' file with the given path and fills the given ParticleContainer with a cuboid of particles
     * using the arguments stored in the file (see README.md or the extensive description for details on the .cub file format)
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     *
     * Reads the '.cub' file with the given path and fills the given ParticleContainer with cuboids of particles
     * using the arguments stored in the file.
     * The file format is as follows:
     * Lines of comment start with '#' and are only allowed in between the argument packs.
     * Afterwards the arguments for the next cuboid start, where each line contains one argument.
     * The arguments for a single cuboid have to be listed in the following order and may not be interrupted by comments:
     * 1. The coordinate of the lower left front-side corner: '<double> <double> <double>'
     * 2. Number of particles per dimension: '<int> <int> <int>'
     * 3. Distance of the particles (mesh width): '<double>'
     * 4. Mass of a single particle: '<double>'#
     * 5. Initial velocity of the particles: '<double> <double> <double>'
     * 6. Mean-value of the velocity of Brownian Motion (for temperature dependent wobbling): '<double>'
     *
     * Multiple cuboids can be defined in one file but must be seperated by at least a newline.
     * In between the full cuboid definitions, comments and newlines are allowed.
     *
     * Example:
     * '''
     * # This is a comment
     * # This is another comment
     * 0.0 0.0 0.0
     * 10 10 10
     * 0.1
     * 1.0
     * 0.4 0.0 0.0
     * 0.0
     *
     * # This is a comment
     * # This is another comment
     * 0.0 0.0 0.0
     * 10 10 10
     * 0.1
     * 1.0
     * 0.4 0.0 0.0
     * 0.0
     * '''
     */
    void readFile(const std::string& filepath, ParticleContainer& particle_container) override;

   private:
    /**
     * @brief Reports an invalid entry in the given file
     * @param filepath Path to the file
     * @param expected_format Expected format of the entry
     * @param actual The entry that was read
     */
    void reportInvalidEntry(const std::string& filepath, const std::string& expected_format, const std::string& actual);
};