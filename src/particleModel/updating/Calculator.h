
#pragma once

#include <functional>
#include <string>

#include "particleModel/storage/SimulationContainer.h"
#include "utils/ForceCalculations.h"

using ForceCalculation =
    std::function<std::array<double, 3>(const Particle&, const Particle&)>;

class Calculator {
    public:
  Calculator(SimulationContainer& simulationContainer,
             const std::string forceType, const double delta_t)
      : simulationContainer(simulationContainer), delta_t(delta_t) {
    if (forceType == "LennJones") {
      // preliminary hardcoded
      double sigma{1.0};
      double epsilon{5.0};
      forceLambda = forceLennJonesPotentialFunction(sigma, epsilon);

    } else if (forceType == "simple") {
      forceLambda = forceSimpleGravitational();

    } else {
      throw std::runtime_error("Provided forceType is invalid: " + forceType);
    }
  }

  /**
   *
   * @brief calculates the force of every Particle at the current time step
   *
   * for every particle all the forces acting on that particle are summed up,
   * while iterating over all unique pairs of particles {p_i,p_j}, where p_i !=
   * p_j. Meaning for the pair {p_i,p_j}, the force f_i_j is calulated and added
   * / substracted to F_i / F_j (on which added/substracted depends on order,in
   * which iterated over the particles) After that the next f_i_j for the next
   * unique pair is calculated sigma and epsilon are only needed to
   * Lennard-Jones-Potential
   *
   * @param forcecalc The function with which the forces between every two
   * particles are calculated
   * @return None
   *
   */
  virtual void calculateF() = 0;

  /**
   *
   * @brief calculates the position of every particle for the next timestep
   * according to given formula
   *
   *
   * @param None
   * @return None
   */
  virtual void calculateX() = 0;

  /**
   * @brief calculates the velocity of every particle for the next timestep
   * according to given formula
   *
   * @param None
   * @return None
   */
  virtual void calculateV() = 0;

  /**
   *
   * @brief for every particle change old force attribute to the current force
   *
   * called after every iteration/ when new X,V and F was calculated
   * sets  the Particle.old_f (old force) for the next time step to be the
   * Particle.f (current force) of this time step
   *
   * @param None
   * @return None
   *
   */
  virtual void shiftForces() = 0;

 protected:
  SimulationContainer& simulationContainer;
  const double delta_t;
  ForceCalculation forceLambda;
};