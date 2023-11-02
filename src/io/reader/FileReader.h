/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "models/Particle.h"
#include "models/ParticleContainer.h"

#include <list>

class FileReader {

public:
  FileReader();
  virtual ~FileReader();

  static void readFile(ParticleContainer &particles, const std::string &filename);
};
