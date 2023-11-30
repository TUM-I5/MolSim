#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <unordered_set>

#include "Particle.h"
#include "outputWriter/VTKWriter.h"
#include "particleModel/storage/SimulationContainer.h"


/**
 * @brief stores an arbitrary amount of particles
 * 
 * The ParticleContainer class uses an std::vector to store an arbitrary amount of particles.
 * It offers an Iterator to iterate over particles or over all unique pairs of particles.
 * 
*/
class ParticleContainer : public SimulationContainer {
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

  Particle *getNextParticle() override;

  void setNextPair(std::pair<Particle *, Particle *> &pair) override;

  void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg,
                   double m_arg) override;

  void reserve(size_t) override;                 

  Particle &operator[](size_t) override;

  void plotParticles(outputWriter::VTKWriter &writer) override;

  std::string to_string() override;

  size_t size() const override;
};


