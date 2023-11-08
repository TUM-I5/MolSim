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
      std::array<double,3> x, v;
      uint64_t N1, N2, N3;
      double m , h, sigma, epsilon;
      double avg_v = 0.1;
  };


  void readParticleFile(ParticleContainer &particleContainer, char *filename);


  void readCuboidFile();
};
