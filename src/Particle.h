/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>
#include "math/LinAlg/Vec.h"

class Particle {

private:
  /**
   * Position of the particle
   */
  LA::Vec<double, 3> x;

  /**
   * Velocity of the particle
   */
  LA::Vec<double, 3> v;

  /**
   * Force effective on this particle
   */
  LA::Vec<double, 3> f;

  /**
   * Force which was effective on this particle
   */
  LA::Vec<double, 3> old_f;

  /**
   * Mass of this particle
   */
  double m;

  /**
   * Type of the particle. Use it for whatever you want (e.g. to separate
   * molecules belonging to different bodies, matters, and so on)
   */
  int type;

public:
  explicit Particle(int type = 0);

  Particle(const Particle &other);

  Particle(
      // for visualization, we need always 3 coordinates
      // -> in case of 2d, we use only the first and the second
      std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
      int type = 0);

  virtual ~Particle();

  const LA::Vec<double, 3> &getX() const;

  const LA::Vec<double, 3> &getV() const;

  const LA::Vec<double, 3> &getF() const;

  const LA::Vec<double, 3> &getOldF() const;

  double getM() const;

  int getType() const;

  void setF(const LA::Vec<double, 3>&);

  void setOldF(const LA::Vec<double, 3>&);

  void add_to_F(const LA::Vec<double, 3>&);

  void add_to_X(const LA::Vec<double, 3>&);

  void add_to_V(const LA::Vec<double, 3>&);

  bool operator==(Particle &other);

  std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
