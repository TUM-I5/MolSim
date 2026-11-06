#include <nlohmann/json.hpp>

#include "JsonReader.h"
#include "ParticleContainer.h"
#include "utils/MaxwellBoltzmannDistribution.h"

namespace file_reader {

using json = nlohmann::json;

Vec3 readVec3(const json &value, const std::string &field_name) {
    if (!value.is_array() || value.size() != 3) {
        throw std::invalid_argument("'" + field_name + "' must be an array of exactly 3 numbers");
    }

    Vec3 result{};
    for (std::size_t i = 0; i < 3; ++i) {
        result[i] = value.at(i).get<double>();
        if (!std::isfinite(result[i])) {
            throw std::invalid_argument("'" + field_name + "' values must be finite");
        }
    }

    return result;
}

std::size_t readPositiveCount(const json &value, const std::string &field_name) {
    if (!value.is_number_integer()) {
        throw std::invalid_argument("'" + field_name + "' must be a positive integer");
    }

    // Read as a signed value first so negative counts are rejected.
    const long long count = value.get<long long>();
    if (count <= 0) {
        throw std::invalid_argument("'" + field_name + "' must be greater than zero");
    }

    return static_cast<std::size_t>(count);
}

void generateParticlesInCuboid(ParticleContainer &particles, const json &cuboid) {
    const Vec3 origin = readVec3(cuboid.at("origin"), "origin");
    const Vec3 initial_velocity = readVec3(cuboid.at("initial_velocity"), "initial_velocity");

    const auto &counts = cuboid.at("particle_counts");
    if (!counts.is_array() || counts.size() != 3) {
        throw std::invalid_argument("'particle_counts' must be an array of exactly 3 integers");
    }
    const std::size_t n1 = readPositiveCount(counts.at(0), "particle_counts[0]");
    const std::size_t n2 = readPositiveCount(counts.at(1), "particle_counts[1]");
    const std::size_t n3 = readPositiveCount(counts.at(2), "particle_counts[2]");

    const double spacing = cuboid.at("spacing").get<double>();
    if (!std::isfinite(spacing) || spacing <= 0.0) {
        throw std::invalid_argument("'spacing' must be finite and greater than zero");
    }

    const double mass = cuboid.at("mass").get<double>();
    if (!std::isfinite(mass) || mass <= 0.0) {
        throw std::invalid_argument("'mass' must be finite and greater than zero");
    }

    const double temperature = cuboid.at("temperature").get<double>();
    if (!std::isfinite(temperature) || temperature < 0.0) {
        throw std::invalid_argument("'temperature' must be equal or greater than zero");
    }

    for (std::size_t i = 0; i < n1; ++i) {
        for (std::size_t j = 0; j < n2; ++j) {
            for (std::size_t k = 0; k < n3; ++k) {
                Vec3 position{origin[0] + static_cast<double>(i) * spacing,
                              origin[1] + static_cast<double>(j) * spacing,
                              origin[2] + static_cast<double>(k) * spacing};

                const auto randomVelocity = maxwellBoltzmannDistributedVelocity(0.1, 3);

                Vec3 velocity = initial_velocity + randomVelocity;

                particles.add(position, velocity, mass);
            }
        }
    }
}

// TODO think about units (escpacially relationship between sigma and h, what is epsilon mean on the
// worksheet)

void JsonReader::Read(ParticleContainer &particles) {
    std::ifstream input_file(file_path_);
    if (!input_file) {
        throw std::invalid_argument("Could not open file: " + file_path_);
    }

    const json config = json::parse(input_file);

    if (config.at("version").get<int>() != 1) {
        throw std::invalid_argument("Unsupported JSON config version");
    }

    const auto &cuboids = config.at("cuboids");
    if (!cuboids.is_array()) {
        throw std::invalid_argument("'cuboids' must be an array");
    }

    for (const auto &cuboid : cuboids) {
        generateParticlesInCuboid(particles, cuboid);
    }
}

}  // namespace file_reader
