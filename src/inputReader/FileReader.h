#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "ParticleContainer.h"

namespace file_reader {

enum class InputFormat { txt, json };

/**
 * @brief Interface for reading input files
 */
class FileReader {
   public:
    virtual ~FileReader() = default;

    /**
     * @brief Reads particles from the input file into the container.
     * @param particles Container to which the particles are added.
     */
    virtual void Read(ParticleContainer &particles) = 0;

    /**
     * @brief Factory for a Reader for the filetype (specified in the file_path)
     *
     * @param file_path File format
     * @return Reader for the filetype (specified in the file_path)
     */
    static std::unique_ptr<FileReader> MakeReader(const std::string file_path);
};

}  // namespace file_reader
