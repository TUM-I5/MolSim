/*
 *  SingleParticleGraviationalForce.cpp
 *
 *  Created on: 15.12.2022
 *      Author: wohlrapp
 */

#include "./SingleParticleGravitationalForce.h"
#include "../model/ParticleContainer.h"
#include "../utils/ArrayUtils.h"

#include <vector>

void SingleParticleGravitationalForce::calculateForce(ParticleContainer &particleContainer, double g_grav)
{
    // we iterate over each particle once and then apply the gravitational force
    std::function<void(Particle &)> forceCalculation = [g_grav](Particle &p1)
    {
        double force = p1.getM() * g_grav; 
        p1.addF({0,force,0});
    };

    particleContainer.iterateParticles(forceCalculation);
}