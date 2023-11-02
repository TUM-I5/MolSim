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

  /**
   * @brief Write all particles in a ParticleContainer to a file in a given location
   *
   * @param particles
   * @param filename
   * @param iteration
   */
  void plotParticles(ParticleContainer &particles, const std::string &filename,
                     int iteration) override;
};

} // namespace outputWriter
