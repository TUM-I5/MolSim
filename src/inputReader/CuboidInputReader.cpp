/*
 * CuboidInputReader.cpp
 *
 *  Created on: 16.11.2022
 *      Author: borisov
 */

#include "./CuboidInputReader.h"
#include "../utils/MaxwellBoltzmannDistribution.h"
#include "../utils/ArrayUtils.h"

#include <string>
#include <fstream>
#include <sstream>

void CuboidInputReader::readInput(ParticleContainer& particleContainer, const char *filename) {
    // Variables to read in
    std::array<double, 3> x;
    std::array<int, 3> n;
    double h;
    double m;
    std::array<double, 3> v;
    double meanV;

    std::ifstream input_file(filename);
    std::string tmp_string;

    if (input_file.is_open()) {
        // skip comments plus cuboid file indicator
        while (tmp_string.empty() or tmp_string[0] == '#' or tmp_string[0] == '$') {
            getline(input_file, tmp_string);
            getLogicLogger()->info("Read line: {}", tmp_string);
        }
        // tmp_string now contains the x
        std::istringstream datastream(tmp_string);

        for (auto &xi : x) {
            datastream >> xi;
        }
        datastream.clear();

        // get next line which contains the n
        getline(input_file, tmp_string);
        getLogicLogger()->info("Read line: {}", tmp_string);
        datastream.str(tmp_string);
        
        for (auto &ni : n) {
            datastream >> ni;
        }
        datastream.clear();

        // get next line which contains the h
        getline(input_file, tmp_string);
        getLogicLogger()->info("Read line: {}", tmp_string);
        datastream.str(tmp_string);

        datastream >> h;
        datastream.clear();

        // get next line which contains the m
        getline(input_file, tmp_string);
        getLogicLogger()->info("Read line: {}", tmp_string);
        datastream.str(tmp_string);

        datastream >> m;
        datastream.clear();

        // get next line which contains the v
        getline(input_file, tmp_string);
        getLogicLogger()->info("Read line: {}", tmp_string);
        datastream.str(tmp_string);

        for (auto &vi : v) {
            datastream >> vi;
        }
        datastream.clear();

        // get next line which contains the meanV
        getline(input_file, tmp_string);
        getLogicLogger()->info("Read line: {}", tmp_string);
        datastream.str(tmp_string);

        datastream >> meanV;
        datastream.clear();
    } else {
        getLogicLogger()->error("Error: could not open file {}", filename);
        exit(-1);
    }

    std::unique_ptr<Cuboid> cuboid = std::make_unique<Cuboid>(Cuboid(x, n, h, m, v, meanV));
    createParticles(particleContainer, *cuboid);
}


void CuboidInputReader::createParticles(ParticleContainer& particleContainer, Cuboid& cuboid) {
    // Dimension 
    int dimension = 2;

    // Variable init
    std::array<double, 3> lowerLeftCorner = cuboid.getX();
    std::array<double, 3> initV = cuboid.getV();
    double m = cuboid.getM();
    
    std::array<int, 3> n = cuboid.getN();
    int numParticles = n[0] * n[1] * n[2];

    double meshWidth = cuboid.getH();

    // reserve memory for particles 
    particleContainer.reserveMemoryForParticles(numParticles);

    // initialize brownian motion
    std::array<double, 3> maxwellBoltzmann = maxwellBoltzmannDistributedVelocity(cuboid.getMeanV(), dimension);
    initV = initV + maxwellBoltzmann;

    // create particles
    std::array<double, 3> position;
    for (int x = 0; x < n[0]; x++) {
        for (int y = 0; y < n[1]; y++) {
            for (int z = 0; z < n[2]; z++) {
                position[0] = lowerLeftCorner[0] + (x * meshWidth);
                position[1] = lowerLeftCorner[1] + (y * meshWidth);
                position[2] = lowerLeftCorner[2] + (z * meshWidth);
                particleContainer.addParticle(position, initV, m);
            }
        }
    }
}