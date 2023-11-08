/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <list>

#include "Particle.h"
#include "ParticleContainer.h"

class FileReader {
 public:
  FileReader();
  virtual ~FileReader();

  struct CuboidData {
    std::array<double, 3> x, v;
    uint64_t N1, N2, N3;
    double m, h, sigma, epsilon;
    double avg_v = 0.1;


    std::string to_string() const {
    std::stringstream ss;

    ss << "CuboidData:" << std::endl;
    ss << "  x: (" << x[0] << ", " << x[1] << ", " << x[2] << ")" << std::endl;
    ss << "  v: (" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl;
    ss << "  N1: " << N1 << std::endl;
    ss << "  N2: " << N2 << std::endl;
    ss << "  N3: " << N3 << std::endl;
    ss << "  m: " << m << std::endl;
    ss << "  h: " << h << std::endl;
    ss << "  sigma: " << sigma << std::endl;
    ss << "  epsilon: " << epsilon << std::endl;

    return ss.str();
  }
  };

  void readParticleFile(ParticleContainer &particleContainer, char *filename);

  std::list<FileReader::CuboidData> readCuboidFile(char *filename);
};
