/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>

class Particle {
 private:
  /**
   * Position of the particle
   */
  std::array<double, 3> x;

  /**
   * Velocity of the particle
   */
  std::array<double, 3> v;

  /**
   * Mass of this particle
   */
  double m;

  /**
   * Type of the particle. Use it for whatever you want (e.g. to separate
   * molecules belonging to different bodies, matters, and so on)
   */
  int type;

  /**
   * Force (which was) effective on this particle
   */
  std::array<double, 3> f_1;

  /**
   * Force (which was) effective on this particle
   */
  std::array<double, 3> f_2;

  /**
   * Used for switching between f1 and f2, determines which was effective on
   * this particle meaning which force vector is referring to the current old
   * force (f2 in the beginning)
   */
  bool secondIsOld = true;

  /**
   * if enabled , Particles functions will show debug output,
   * like messages for creation and destruction of Particles
  */
  static const bool verbose_particle = false;

 public:
  explicit Particle(int type = 0);

  Particle(const Particle &other);

  Particle(
      // for visualization, we need always 3 coordinates
      // -> in case of 2d, we use only the first and the second
      std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
      int type = 0);

  virtual ~Particle();

  const std::array<double, 3> &getX() const;

  const std::array<double, 3> &getV() const;

  const std::array<double, 3> &getF() const;

  const std::array<double, 3> &getOldF() const;

  double getM() const;

  int getType() const;

  void setX(int index, double value);

  void setV(int index, double value);

  void addF(int index, double value);

  void shiftF();

  bool operator==(Particle &other);

  std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
