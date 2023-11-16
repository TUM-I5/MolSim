#pragma once

#include "ParticleContainer.h"
#include <functional>
/**
 * using a lambda function interface for the calcualtion of the force between two
 * Particles, takes in two Particles and returns the vector3 of forces
 * acting between the two given Particles
 * simplified:
 * forceCalculation refers to such functions "std::array<double,3> func(const Particle&,const Particle&)"
 * uses constant references because forceCalculation mustn't change the Particles
 */
using ForceCalculation = std::function<std::array<double, 3>(const Particle &, const Particle &)>;


class Model {
public:
    Model(ParticleContainer& particleContainer, const std::string& forceType, const double delta_t);

    /**
     *
     * @brief calculates the force of every Particle at the current time step
     *
     * for every particle all the forces acting on that particle are summed up, while
     * iterating over all unique pairs of particles {p_i,p_j}, where p_i != p_j. Meaning
     * for the pair {p_i,p_j}, the force f_i_j is calulated and added / substracted
     * to F_i / F_j (on which added/substracted depends on order,in which iterated over the particles)
     * After that the next f_i_j for the next unique pair is calculated
     * sigma and epsilon are only needed to Lennard-Jones-Potential
     *
     * @param forcecalc The function with which the forces between every two particles are calculated
     * @return None
     *
     */
    void calculateF();

    /**
     *
     * @brief calculates the position of every particle for the next timestep according to given formula
     *
     *
     * @param None
     * @return None
     */
    void calculateX();

    /**
     * @brief calculates the velocity of every particle for the next timestep according to given formula
     *
     * @param None
     * @return None
     */
    void calculateV();

    /**
     *
     * @brief for every particle change old force attribute to the current force
     *
     * called after every iteration/ when new X,V and F was calculated
     * sets  the Particle.old_f (old force) for the next time step to be the Particle.f (current force) of this
     * time step
     *
     * @param None
     * @return None
     *
     */
    void shiftForces();

    int calculateTimesForce=0;
    int calculateTimesVelocity=0;
    int calculateTimesPosition=0;

private:
    const double delta_t;
    ParticleContainer& particleContainer;
    ForceCalculation forceLambda;

};