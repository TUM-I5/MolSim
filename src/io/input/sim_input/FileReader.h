/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "data/Particle.h"
#include "io/input/arg_names.h"

#include <list>

namespace io::input {
    class FileReader {

    public:
        FileReader();

        virtual ~FileReader();

        static void readFile(const char *filename, std::list<Particle> &particles, std::unordered_map<io::input::names, std::string> &);
    };
} // io::input
