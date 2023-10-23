/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <list>

#include "Particle.h"

class FileReader {
   public:
    FileReader();
    virtual ~FileReader();

    void readFile(std::list<Particle> &particles, std::string& filepath);
};
