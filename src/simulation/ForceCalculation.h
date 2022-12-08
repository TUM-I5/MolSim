/*
 *  ForceCalculation.h
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#pragma once

#include "../model/ParticleContainer.h"

/**
 * @brief abstract class for force calculation
 */
class ForceCalculation
{
private:
    /**
     * a speedlog logger which logs construction and destruction of particles
     */
    std::shared_ptr<spdlog::logger> _memoryLogger;

public:
    ForceCalculation();

    virtual ~ForceCalculation() = 0;

    /**
     * @brief calculates the force between particles
     *
     * @param particleContainer container for the particles for which the force will be calculated
     */
    virtual void calculateForce(ParticleContainer &particleContainer) = 0;
};