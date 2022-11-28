/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "data/Particle.h"
#include "data/ParticleContainer.h"

#include <fstream>
#include <vector>

namespace outputWriter {

class XYZWriter {

public:
  XYZWriter();

  virtual ~XYZWriter();

  void plotParticles(ParticleContainer& container, const std::string &filename,
                     int iteration);
};

} // namespace outputWriter
