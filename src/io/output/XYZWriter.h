/**
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include <fstream>
#include <list>

#include "types/Particle.h"

class XYZWriter {
   public:
    XYZWriter();

    virtual ~XYZWriter();

    void plotParticles(std::list<Particle> particles, const std::string& filename, int iteration);
};
