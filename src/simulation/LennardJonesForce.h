/*
 * LennardJonesSimulation.h
 *
 * Created: 16.11.2022
 * Author:  marquardt
 */

#pragma once

#include "ForceCalculation.h"

/**
 * @brief Provides the force calculation according to Lennard-Jones potential
 */
class LennardJonesForce : public ForceCalculation
{

public:
    LennardJonesForce(double sigma, double epsilon) {
        _sigma = sigma;
        _epsilon = epsilon;
    }
    
private:
    double _sigma;
    double _epsilon;
    /**
     * @brief Calculate the force for all particles
     *
     * @param particleContainer The container which contains the particles
     */
    void calculateForce(ParticleContainer &particleContainer) override;
};