#pragma once

#include <iostream>
#include <list>
#include <vector>

#include "Particle.h"
#include "outputWriter/VTKWriter.h"

class ParticleContainer {
 private:
  /**
   * internal Data structure for storing particles(can be changed if interfaces
   * of this class are changed accordingly)
   */
  std::vector<Particle> particles;

 public:
  /**
   * Constructor and Destructor
   */

  explicit ParticleContainer(const std::list<Particle> &part_list);

  ParticleContainer();

  virtual ~ParticleContainer();

  /**
   * Functions acting on ParticleContainer
   */

  Particle *getNextParticle();

  void setNextPair(std::pair<Particle *, Particle *> &pair);

  void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg,
                   double m_arg);

  Particle &operator[](size_t);

  void plotParticles(outputWriter::VTKWriter &writer);

  void printParticles();

  size_t size() const;
};
