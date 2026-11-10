/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "TxtReader.h"

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "ParticleContainer.h"

namespace file_reader {

void TxtReader::Read(ParticleContainer &particles) {
    if (std::ifstream input_file(file_path_); input_file.is_open()) {
        Vec3 x{};
        Vec3 v{};
        double m;
        int num_particles = 0;
        std::string tmp_string;

        getline(input_file, tmp_string);
        SPDLOG_TRACE("Read line: {}", tmp_string);

        while (tmp_string.empty() or tmp_string[0] == '#') {
            getline(input_file, tmp_string);
            SPDLOG_TRACE("Read line: {}", tmp_string);
        }

        std::istringstream numstream(tmp_string);
        numstream >> num_particles;
        SPDLOG_DEBUG("Reading: {}.", num_particles);
        getline(input_file, tmp_string);
        SPDLOG_TRACE("Read line: {}", tmp_string);

        for (int i = 0; i < num_particles; i++) {
            std::istringstream datastream(tmp_string);

            for (auto &xj : x) {
                datastream >> xj;
            }
            for (auto &vj : v) {
                datastream >> vj;
            }
            if (datastream.eof()) {
                throw std::runtime_error(fmt::format("unexpected end of file in line {}", i));
            }
            datastream >> m;
            particles.add(x, v, m);

            getline(input_file, tmp_string);
            SPDLOG_TRACE("Read line: {}", tmp_string);
        }
    } else {
        throw std::runtime_error(fmt::format("Error: could not open file {}", file_path_));
    }
}

}  // namespace file_reader
