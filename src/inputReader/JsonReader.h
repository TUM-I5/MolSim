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
 *
 * Format:
 *  - `version`: format version
 *  - `cuboids`: array of cuboids, each one generating a 3D grid of particles
 *    - `origin`: The min min min corner `[x, y, z]`
 *    - `particle_counts`: number of particles in dimensions `[N1, N2, N3]`, all > 0
 *    - `spacing`: distance between neighboring particles, > 0
 *    - `mass`: mass of each particle, > 0
 *    - `initial_velocity`: velocity of the whole cuboid `[vx, vy, vz]`
 *    - `average_velocity`: mean vel of the brownian motion, >= 0
 *
 * Example:
 * @code
 * {
 *   "version": 1,
 *   "cuboids": [
 *     {
 *       "origin": [0.0, 0.0, 0.0],
 *       "particle_counts": [40, 8, 1],
 *       "spacing": 1.1225,
 *       "mass": 1.0,
 *       "initial_velocity": [0.0, 0.0, 0.0],
 *       "average_velocity": 0.1
 *     },
 *     {
 *       "origin": [15.0, 15.0, 0.0],
 *       "particle_counts": [8, 8, 1],
 *       "spacing": 1.1225,
 *       "mass": 1.0,
 *       "initial_velocity": [0.0, -10.0, 0.0],
 *       "average_velocity": 0.1
 *     }
 *   ]
 * }
 * @endcode
 */
class JsonReader final : public FileReader {
    const std::string file_path_;

   public:
    /**
     * @brief Constructs a reader for the specified JSON file.
     * @param file_path Path to the input JSON file.
     */
    explicit JsonReader(std::string file_path) : file_path_(std::move(file_path)) {}

    /**
     * @brief Adds particles read from the JSON file to the container.
     * @param particles Container to which the particles are added.
     */
    void Read(ParticleContainer &particles) override;
};

}  // namespace file_reader
