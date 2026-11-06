#pragma once

#include <fstream>
#include <string>

#include "FileReader.h"
#include "ParticleContainer.h"

namespace file_reader {

/**
 * @brief Reads particle data from a JSON file.
 *
 * The input file path is supplied when the reader is constructed.
 */
class JsonReader final : public FileReader {
    const std::string file_path_;

   public:
    /**
     * @brief Constructs a reader for the specified JSON file.
     * @param file_path Path to the input JSON file.
     */
    explicit JsonReader(const std::string file_path) : file_path_(file_path) {}

    /**
     * @brief Adds particles read from the JSON file to the container.
     * @param particles Container to which the particles are added.
     */
    void Read(ParticleContainer &particles) override;
};

}  // namespace file_reader
