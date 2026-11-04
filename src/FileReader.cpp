#include "FileReader.h"

#include <cmath>
#include <cstddef>
#include <fstream>
#include <limits>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

#include "ParticleContainer.h"
#include "utils/MaxwellBoltzmannDistribution.h"

namespace {

using json = nlohmann::json;

/**
 * Read a three-component vector from a JSON value.
 *
 * @param value JSON array to read.
 * @param field_name Name of the field, used in error messages.
 * @return The three-component vector.
 * @throws std::runtime_error If the value is not an array of exactly three
 *         finite numbers.
 */
Vec3 readVec3(const json &value, const std::string &field_name) {
    if (!value.is_array() || value.size() != 3) {
        throw std::runtime_error("'" + field_name + "' must be an array of exactly 3 numbers");
    }

    Vec3 result{};
    for (std::size_t i = 0; i < 3; ++i) {
        result[i] = value.at(i).get<double>();
        if (!std::isfinite(result[i])) {
            throw std::runtime_error("'" + field_name + "' values must be finite");
        }
    }

    return result;
}

/**
 * @brief Reads a positive integer count from a JSON value.
 * @param value JSON value to read.
 * @param field_name Field name used in error messages.
 * @return The count.
 * @throws std::runtime_error If the value is not a positive integer.
 */
std::size_t readPositiveCount(const json &value, const std::string &field_name) {
    if (!value.is_number_integer()) {
        throw std::runtime_error("'" + field_name + "' must be a positive integer");
    }

    // Read as a signed value first so negative counts are rejected.
    const long long count = value.get<long long>();
    if (count <= 0) {
        throw std::runtime_error("'" + field_name + "' must be greater than zero");
    }

    return static_cast<std::size_t>(count);
}

/**
 * @brief Generates particles arranged in a 3D grid within a cuboid.
 * @param particles Container to which the generated particles are added.
 * @param cuboid JSON object containing the cuboid and particle properties.
 */
void generateParticlesInCuboid(ParticleContainer &particles, const json &cuboid) {
    const Vec3 origin = readVec3(cuboid.at("origin"), "origin");
    const Vec3 initial_velocity = readVec3(cuboid.at("initial_velocity"), "initial_velocity");

    const auto &counts = cuboid.at("particle_counts");
    if (!counts.is_array() || counts.size() != 3) {
        throw std::runtime_error("'particle_counts' must be an array of exactly 3 integers");
    }
    const std::size_t n1 = readPositiveCount(counts.at(0), "particle_counts[0]");
    const std::size_t n2 = readPositiveCount(counts.at(1), "particle_counts[1]");
    const std::size_t n3 = readPositiveCount(counts.at(2), "particle_counts[2]");

    const double spacing = cuboid.at("spacing").get<double>();
    if (!std::isfinite(spacing) || spacing <= 0.0) {
        throw std::runtime_error("'spacing' must be finite and greater than zero");
    }

    const double mass = cuboid.at("mass").get<double>();
    if (!std::isfinite(mass) || mass <= 0.0) {
        throw std::runtime_error("'mass' must be finite and greater than zero");
    }

    const double temperature = cuboid.at("temperature").get<double>();
    if (!std::isfinite(temperature) || temperature <= 0.0) {
        throw std::runtime_error("'temperature' must be finite and greater than zero");
    }

    for (std::size_t i = 0; i < n1; ++i) {
        for (std::size_t j = 0; j < n2; ++j) {
            for (std::size_t k = 0; k < n3; ++k) {
                Vec3 position{origin[0] + static_cast<double>(i) * spacing,
                              origin[1] + static_cast<double>(j) * spacing,
                              origin[2] + static_cast<double>(k) * spacing};

                const auto randomVelocity =
                    maxwellBoltzmannDistributedVelocity(temperature, mass, 3);

                Vec3 velocity{initial_velocity[0] + randomVelocity[0],
                              initial_velocity[1] + randomVelocity[1],
                              initial_velocity[2] + randomVelocity[2]};

                particles.add(position, velocity, mass);
            }
        }
    }
}

}  // namespace

void FileReader::ReadFile(ParticleContainer &particles, const std::string &filename) {
    std::ifstream input_file(filename);
    if (!input_file) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    const json config = json::parse(input_file);

    if (config.at("version").get<int>() != 1) {
        throw std::runtime_error("Unsupported JSON config version");
    }

    const auto &cuboids = config.at("cuboids");
    if (!cuboids.is_array()) {
        throw std::runtime_error("'cuboids' must be an array");
    }

    for (const auto &cuboid : cuboids) {
        generateParticlesInCuboid(particles, cuboid);
    }
}
