/*
 * GravitySimulation.h 
 * 
 * Created: 31.10.2022 
 * Author:  wohlrapp 
*/

#pragma once 

#include "Simulation.h"

/** 
 * @brief Provides the force calculation according to gravitational laws
 */
class GravitySimulation : public Simulation {

using Simulation::Simulation; // making the constructors of the base class available

private:
    /**
     * @brief Calculate the force for all particles
     */
    virtual void calculateF();
};