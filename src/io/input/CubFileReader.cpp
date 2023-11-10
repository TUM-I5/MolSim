#include "CubFileReader.h"

#include <fstream>
#include <iostream>
#include <limits>

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
        input_file >> x >> y >> z;
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double> <double> <double>", tmp_string);
        }
        std::array<double, 3> lower_left_front_corner{x, y, z};

        int nx, ny, nz;
        input_file >> nx >> ny >> nz;
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<int> <int> <int>", tmp_string);
        }
        std::array<int, 3> num_particles_per_dimension{nx, ny, nz};

        double mesh_width;
        input_file >> mesh_width;
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double>", tmp_string);
        }

        double mass;
        input_file >> mass;
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double>", tmp_string);
        }

        double vx, vy, vz;
        input_file >> vx >> vy >> vz;
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double> <double> <double>", tmp_string);
        }
        std::array<double, 3> initial_velocity{vx, vy, vz};

        double mean_wobbling_velocity;
        input_file >> mean_wobbling_velocity;
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<double>", tmp_string);
        }

        int type;
        input_file >> type;
        if (input_file.fail()) {
            reportInvalidEntry(filepath, "<int>", tmp_string);
        }

        std::cout << "lower_left_front_corner: " << lower_left_front_corner[0] << " " << lower_left_front_corner[1] << " " << lower_left_front_corner[2] << std::endl;
        std::cout << "num_particles_per_dimension: " << num_particles_per_dimension[0] << " " << num_particles_per_dimension[1] << " " << num_particles_per_dimension[2] << std::endl;
        std::cout << "mesh_width: " << mesh_width << std::endl;
        std::cout << "mass: " << mass << std::endl;
        std::cout << "initial_velocity: " << initial_velocity[0] << " " << initial_velocity[1] << " " << initial_velocity[2] << std::endl;
        std::cout << "mean_wobbling_velocity: " << mean_wobbling_velocity << std::endl;
        std::cout << "type: " << type << std::endl;
    }
}

void CubFileReader::reportInvalidEntry(const std::string& filepath, const std::string& expected_format, const std::string& actual) {
    std::cout << "Error: invalid entry in file '" << filepath << "'." << std::endl;
    std::cout << "       Expected format: '" << expected_format << "'" << std::endl;
    std::cout << "       Got: '" << actual << "'" << std::endl;
    exit(-1);
}