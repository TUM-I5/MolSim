#include "PsFileReader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "io/logger/Logger.h"
#include "particles/containers/directsum/DirectSumContainer.h"

SimulationParams PsFileReader::readFile(const std::string& filepath, std::unique_ptr<ParticleContainer>& particle_container) const {
    std::array<double, 3> x;
    std::array<double, 3> v;
    double m;
    int num_particles = 0;

    std::ifstream input_file(filepath);
    std::string curr_line;

    if (!input_file.is_open()) {
        throw FileFormatException(fmt::format("Error: could not open file '{}'.", filepath));
    }

    getline(input_file, curr_line);

    while (curr_line.empty() or curr_line[0] == '#') {
        getline(input_file, curr_line);
    }

    std::istringstream numstream(curr_line);
    numstream >> num_particles;
    particle_container->reserve(num_particles);
    getline(input_file, curr_line);

    // Initialize particle container
    particle_container = std::make_unique<DirectSumContainer>();

    for (int i = 0; i < num_particles; i++) {
        std::istringstream datastream(curr_line);

        for (auto& xj : x) {
            datastream >> xj;
        }
        for (auto& vj : v) {
            datastream >> vj;
        }
        if (datastream.eof()) {
            throw FileFormatException(fmt::format("Error reading file: eof reached unexpectedly reading from line {}.", i));
        }
        datastream >> m;

        particle_container->addParticle(Particle{x, v, m, i});

        getline(input_file, curr_line);
    }

    return SimulationParams(filepath, "", 0.0002, 5, 24, 30, SimulationParams::DirectSumType(), "vtk");
}
