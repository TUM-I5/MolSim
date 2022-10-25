/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "Particle.h"

#include <list>
#include <vector>

class FileReader {

public:
  FileReader();
  virtual ~FileReader();

  void readFile(std::vector<Particle> &particles, char *filename);
};
