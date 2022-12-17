/*
 *  InterParticleGravitationalForce.h
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#pragma once

#include "InterParticleForce.h"

/**
 * @brief Class calculating the gravitational force
 */
class InterParticleGravitationalForce : public InterParticleForce
{
public:
    /**
     * @brief calculates the gravitational force between particles
     *
     * @param particleContainer container for the particles for which the force will be calculated
     */
    void calculateForce(ParticleContainer &particleContainer) override;
};