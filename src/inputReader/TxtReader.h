/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <string>

#include "FileReader.h"

namespace file_reader {

/**
 * @brief Reader to read initial particle data from txt file
 *
 * The input file path is supplied when the reader is constructed.
 *
 * Format:
 *  - Leading lines starting with '#' are ignored (comments)
 *  - First real line is the number of particles N
 *  - N lines for one particle each. `x1 x2 x3  v1 v2 v3  m`
 *
 * Example:
 * @code
 * # Sun, Earth, Jupiter, Halley
 * 4
 * 0.0 0.0 0.0      0.0 0.0 0.0     1.0
 * 0.0 1.0 0.0     -1.0 0.0 0.0     3.0e-6
 * 0.0 5.36 0.0    -0.425 0.0 0.0   9.55e-4
 * 34.75 0.0 0.0    0.0 0.0296 0.0  1.0e-14
 * @endcode
 */
class TxtReader final : public FileReader {
    const std::string file_path_;

   public:
    /**
     * @brief Constructs a reader for the specified txt file.
     * @param file_path Path to the input txt file.
     */
    explicit TxtReader(std::string file_path) : file_path_(std::move(file_path)) {}

    /**
     * @brief Reads a list of initial particle data from @p file_path and populates @p particles
     * with them
     *
     * Exits the program on a malformed file or cant be opened
     *
     * @param particles Container to fill with particle data
     */
    void Read(ParticleContainer &particles) override;
};

}  // namespace file_reader
