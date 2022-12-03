//
// Created by jan on 11/29/22.
//

#pragma once

#include "data/Particle.h"
#include "data/ParticleGenerator.h"
#include "io/input/arg_names.h"
#include "io/input/sim_input/XMLFormat.h"
#include <memory>
#include "io/output/Logging.h"
#include "defaults.h"


#include <list>

namespace io::input {
    class XMLReader {

    public:
        XMLReader();

        virtual ~XMLReader();

        static void readFile(const char *filename, std::list<Particle> &particles, std::unordered_map<io::input::names, std::string> &);

    private:
        static void dvectorToEigenVector3d(dvector_t const &, Eigen::Vector3d &);

        static void ivectorToEigenVector3d(ivector_t const &, Eigen::Vector3d &);
    };
} // io::input

