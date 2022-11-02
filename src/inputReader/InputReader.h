/*
 * InputReader.h 
 * 
 * Created: 31.10.2022 
 * Author:  wohlrapp 
*/


#pragma once

#include "../model/ParticleContainer.h"

#include <vector>

/**
 * @brief Base class to read input, provides a shared interface
*/
class InputReader {

public:
  /**
   * Abstract function to read the input from a file 
   * @param ParticleContainer& particleContainer, char *filename
  */
  virtual void readInput(ParticleContainer& particleContainer, char *filename)=0;
};