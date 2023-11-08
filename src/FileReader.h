/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "Particle.h"
#include "ParticleContainer.h"

#include <list>

class FileReader {

public:
  FileReader();
  virtual ~FileReader();

  struct CuboidData {
      std::array<double,3> x1 , x2 , v1 , v2;
      std::array<uint64_t,3> N1 , N2 , N3;
      double m , h;
  };


  void readParticleFile(ParticleContainer &particleContainer, char *filename);


  void readCuboidFile();
};
