#pragma once

#include "FileReader.h"

/**
 * @brief Class to read particle data from a '.cub' file
 */
class CubFileReader : public FileReader {
   public:
    /**
     * @brief Reads the '.cub' file with the given path and fills the given ParticleContainer with a cuboid of particles
     * using the arguments stored in the file (see README.md or the extensive description for details on the .cub file format)
     * @param filepath Path to the file to be read
     * @param particle_container ParticleContainer to be filled
     *
     * Reads the '.cub' file with the given path and fills the given ParticleContainer with cuboids of particles
     * using the arguments stored in the file (see \ref InputFileFormats "Input File Formats")
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