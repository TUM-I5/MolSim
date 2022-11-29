//
// Created by jan on 11/29/22.
//

#pragma once

#include "data/Particle.h"
#include "io/input/arg_names.h"

#include <list>

namespace io::input {
    class XMLReader {

    public:
        XMLReader();

        virtual ~XMLReader();

        static void readFile(const char *filename, std::list<Particle> &particles, std::unordered_map<io::input::names, std::string> &);
    };
} // io::input

