/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "InputReader.h"
#include "../model/ParticleContainer.h"

class FileReader : public InputReader {

public:
  FileReader();
  virtual ~FileReader();

  virtual void readInput(ParticleContainer& particleContainer, char *filename);
};
