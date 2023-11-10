#include "CubFileReader.h"

#include <fstream>
#include <iostream>
#include <limits>

#include "io/particle_spawners/CuboidSpawner.h"

void CubFileReader::readFile(const std::string& filepath, ParticleContainer& particleContainer) {
    std::ifstream input_file(filepath);

    if (!input_file.is_open()) {
        std::cout << "Error: could not open file '" << filepath << "'." << std::endl;
        exit(-1);
    }

    while (!input_file.eof()) {
        std::string tmp_string;
        while (input_file.peek() == '#' || input_file.peek() == '\n') {
            std::getline(input_file, tmp_string);
        }

        double x, y, z;
        input_file >> x >> y >> z >> std::ws;
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double> <double> <double>", tmp_string);
        }
        std::array<double, 3> lower_left_front_corner{x, y, z};

        size_t nx, ny, nz;
        input_file >> nx >> ny >> nz >> std::ws;
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<int> <int> <int>", tmp_string);
        }
        std::array<size_t, 3> grid_dimensions{nx, ny, nz};

        double grid_spacing;
        input_file >> grid_spacing >> std::ws;
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double>", tmp_string);
        }

        double mass;
        input_file >> mass >> std::ws;
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double>", tmp_string);
        }

        double vx, vy, vz;
        input_file >> vx >> vy >> vz >> std::ws;
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double> <double> <double>", tmp_string);
        }
        std::array<double, 3> initial_velocity{vx, vy, vz};

        double mean_wobbling_velocity;
        input_file >> mean_wobbling_velocity >> std::ws;
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double>", tmp_string);
        }

        int type;
        input_file >> type >> std::ws;
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<int>", tmp_string);
        }

        auto spawner = CuboidSpawner(lower_left_front_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type);

        particleContainer.reserve(particleContainer.size() + nx * ny * nz);
        spawner.spawnParticles(particleContainer);
    }
}

void CubFileReader::reportInvalidEntry(const std::string& filepath, const std::string& expected_format, const std::string& actual) {
    std::cout << "Error: invalid entry in file '" << filepath << "'." << std::endl;
    std::cout << "       Expected format: '" << expected_format << "'" << std::endl;
    std::cout << "       Got: '" << actual << "'" << std::endl;
    exit(-1);
}