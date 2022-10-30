/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "Particle.h"

#include <list>

namespace io {
    class FileReader {

    public:
        FileReader();
        virtual ~FileReader();

        static void readFile( const char *filename, std::list<Particle> &particles);
    };
} // io
