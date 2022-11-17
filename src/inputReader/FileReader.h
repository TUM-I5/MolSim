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

  /**
   * @brief Function to read the input from a file, in this case a text file 
   * @param particleContainer The reference to a wrapper for the particles, when reading the input, new particles can be added to that container
   * @param filename The pointer to the filename of the input file
  */
  virtual void readInput(ParticleContainer& particleContainer, const char *filename);
};
