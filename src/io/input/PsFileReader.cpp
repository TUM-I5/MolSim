/**
 * PsFileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "PsFileReader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

void PsFileReader::readFile(const std::string& filepath, ParticleContainer& particle_container) const {
    std::array<double, 3> x;
    std::array<double, 3> v;
    double m;
    int num_particles = 0;

    std::ifstream input_file(filepath);
    std::string curr_line;

    if (input_file.is_open()) {
        getline(input_file, curr_line);

        while (curr_line.empty() or curr_line[0] == '#') {
            getline(input_file, curr_line);
        }

        std::istringstream numstream(curr_line);
        numstream >> num_particles;
        particle_container.reserve(num_particles);
        getline(input_file, curr_line);

        for (int i = 0; i < num_particles; i++) {
            std::istringstream datastream(curr_line);

            for (auto& xj : x) {
                datastream >> xj;
            }
            for (auto& vj : v) {
                datastream >> vj;
            }
            if (datastream.eof()) {
                std::cout << "Error reading file: eof reached unexpectedly reading from line " << i << std::endl;
                exit(-1);
            }
            datastream >> m;

            particle_container.addParticle(Particle{x, v, m, i});

            getline(input_file, curr_line);
        }
    } else {
        std::cout << "Error: could not open file " << filepath << std::endl;
        exit(-1);
    }
}
