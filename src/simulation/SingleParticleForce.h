/*
 *  SingleParticleForce.h
 *
 *  Created on: 15.12.2022
 *      Author: wohlrapp
 */

#pragma once

#include "../model/ParticleContainer.h"

/**
 * @brief abstract class for force calculation acting on single particles
 */
class SingleParticleForce
{
private:
    /**
     * a speedlog logger which logs construction and destruction of particles
     */
    std::shared_ptr<spdlog::logger> _memoryLogger;

public:
    SingleParticleForce();

    virtual ~SingleParticleForce() = 0;

    /**
     * @brief calculates the force acting on a single particle
     *
     * @param particleContainer container for the particles for which the force will be calculated
     */
    virtual void calculateForce(ParticleContainer &particleContainer, double external_force) = 0;
};