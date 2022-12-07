/*
 * SphereInputReader.cpp
 *
 *  Created on: 04.12.2022
 *      Author: wohlrapp
 */

#include "./SphereInputReader.h"
#include "../utils/MaxwellBoltzmannDistribution.h"
#include "../utils/ArrayUtils.h"
#include "../utils/ParticleGenerator.h"

#include <string>
#include <fstream>
#include <sstream>

void SphereInputReader::readInput(ProgramParameters &programParameters, const char *filename)
{
    // Variables to read in
    std::array<double, 3> center;
    int r;
    double h;
    double m;
    std::array<double, 3> v;
    double meanV;

    std::ifstream input_file(filename);
    std::string tmp_string;

    if (input_file.is_open())
    {
        auto particleContainer = programParameters.getParticleContainer();
        // skip comments plus cuboid file indicator
        while (tmp_string.empty() or tmp_string[0] == '#' or tmp_string[0] == '$')
        {
            getline(input_file, tmp_string);
            getLogicLogger()->info("Read line: {}", tmp_string);
        }
        // tmp_string now contains the x
        std::istringstream datastream(tmp_string);

        for (auto &xi : center)
        {
            datastream >> xi;
        }
        datastream.clear();

        // get next line which contains the n
        getline(input_file, tmp_string);
        getLogicLogger()->info("Read line: {}", tmp_string);
        datastream.str(tmp_string);

        datastream >> r;
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

        for (auto &vi : v)
        {
            datastream >> vi;
        }
        datastream.clear();

        // get next line which contains the meanV
        getline(input_file, tmp_string);
        getLogicLogger()->info("Read line: {}", tmp_string);
        datastream.str(tmp_string);

        datastream >> meanV;
        datastream.clear();
    }
    else
    {
        getLogicLogger()->error("Error: could not open file {}", filename);
        exit(-1);
    }

    std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(Sphere(center, r, h, m, v, meanV));
    ParticleGenerator::generateSphere(particleContainer, *sphere);
}