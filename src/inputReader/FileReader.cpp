/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

FileReader::FileReader() = default;

FileReader::~FileReader() = default;

void FileReader::readInput(ProgramParameters &programParameters, const char *filename)
{
    std::array<double, 3> x;
    std::array<double, 3> v;
    double m;
    int num_particles = 0;

    std::ifstream input_file(filename);
    std::string tmp_string;

    if (input_file.is_open())
    {
        auto particleContainer = programParameters.getParticleContainer();
        getline(input_file, tmp_string);
        getLogicLogger()->info("Read line: {}", tmp_string);

        while (tmp_string.empty() or tmp_string[0] == '#')
        {
            getline(input_file, tmp_string);
            getLogicLogger()->info("Read line: {}", tmp_string);
        }

        std::istringstream numstream(tmp_string);
        numstream >> num_particles;
        getLogicLogger()->info("Reading: {}.", num_particles);
        getline(input_file, tmp_string);
        getLogicLogger()->info("Read line: {}", tmp_string);

        // we know how many particles will be added so we reserve memory in advance to prevent reallocation of particles
        particleContainer->reserveMemoryForParticles(num_particles);

        for (int i = 0; i < num_particles; i++)
        {
            std::istringstream datastream(tmp_string);

            for (auto &xj : x)
            {
                datastream >> xj;
            }
            for (auto &vj : v)
            {
                datastream >> vj;
            }
            if (datastream.eof())
            {
                getLogicLogger()->error("Error reading file: eof reached unexpectedly reading from line {}", i);
                exit(-1);
            }
            datastream >> m;
            particleContainer->addParticle(x, v, m);

            getline(input_file, tmp_string);
            getLogicLogger()->info("Read line: {}", tmp_string);
        }
    }
    else
    {
        getLogicLogger()->error("Error: could not open file {}", filename);
        exit(-1);
    }
}
