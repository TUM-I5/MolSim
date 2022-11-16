/*
 * LennardJonesSimulation.h 
 * 
 * Created: 16.11.2022 
 * Author:  marquardt 
*/

#pragma once 

#include "Simulation.h"

/** 
 * @brief Provides the force calculation according to Lennard-Jones potential
 */
class LennardJonesSimulation : public Simulation {

using Simulation::Simulation; // making the constructors of the base class available

private:
    /**
     * @brief Calculate the force for all particles
     */
    void calculateF() override;
};