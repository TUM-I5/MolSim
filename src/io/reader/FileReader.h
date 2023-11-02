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

  /**
   * @brief Read particles from a file into a ParticleContainer
   *
   * @param particles
   * @param filename
   */
  static void readFile(ParticleContainer &particles, const std::string &filename);
};