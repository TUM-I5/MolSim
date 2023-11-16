/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>

/**
 * Particle class
 * Represents a particle in a simulation
 */
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
   * Force effective on this particle
   */
  std::array<double, 3> f;

  /**
   * Force which was effective on this particle
   */
  std::array<double, 3> old_f;

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

  const std::array<double, 3> &getX() const;

  const std::array<double, 3> &getV() const;

  const std::array<double, 3> &getF() const;

  const std::array<double, 3> &getOldF() const;

  double getM() const;

  int getType() const;

  bool operator==(Particle &other);

  std::string toString() const;

  /**
   * @brief Calculates the distance between the current particle and another one
   *
   * @param particle
   * @return Distance to another particle
   */
double distanceTo(Particle &particle);

/**
 * @brief Calculates the distance between the current particle and another one
 *
 * @param particle
 * @return
 */
std::array<double, 3> diffTo(Particle &particle);

void setOldF(const std::array<double, 3> &old_f_arg);

void setF(const std::array<double, 3> &f_arg);

void setV(const std::array<double, 3> &v_arg);

void setX(const std::array<double, 3> &x_arg);

/**
 * @brief Move f to oldF and give f a new value
 *
 * @param f_arg
 */
void updateF(const std::array<double, 3> &f_arg);
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
