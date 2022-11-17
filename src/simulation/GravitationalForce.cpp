/*
 *  GravitationalForce.cpp
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#include "GravitationalForce.h"
#include "../model/ParticleContainer.h"
#include "../utils/ArrayUtils.h"

#include <vector>

void GravitationalForce::calculateForce(ParticleContainer &particleContainer) {
    // first we iterate over each particle once to initialize new force vector to zero
    std::function<void (Particle &)> forceInitializationIteration = [] (Particle &p1) {
        p1.setOldF(p1.getF());
        p1.setF({0.0 , 0.0, 0.0});
    };

    particleContainer.iterateParticles(forceInitializationIteration); 

    // in the second step we calculate the forces between pairs of particles according to the formula
    std::function<void (Particle&, Particle&)> forceCalculationIteration = [] (Particle &p1, Particle &p2) {
        double distance = ArrayUtils::L2Norm(p1.getX() - p2.getX());

        std::array<double,3> f_ij = (p1.getM() * p2.getM() / pow(distance,3)) * (p2.getX() - p1.getX());
        std::array<double,3> f_ji = -1 * f_ij;

        p1.addF(f_ij); 
        p2.addF(f_ji);
    };

    particleContainer.iterateParticlePairs(forceCalculationIteration);
}