/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "io/outputWriter/Writer.h"

#include <fstream>
#include <list>

namespace outputWriter {

class XYZWriter : public Writer {

public:
  XYZWriter();

  virtual ~XYZWriter();

  void plotParticles(ParticleContainer &particles, const std::string &filename,
                     int iteration) override;
};

} // namespace outputWriter
