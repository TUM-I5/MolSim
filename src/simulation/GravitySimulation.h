/*
 * GravitySimulation.h 
 * 
 * Created: 31.10.2022 
 * Author:  wohlrapp 
*/

#pragma once 

#include "Simulation.h"

/** 
 * @brief Force calculation according to gravitational laws
*/
class GravitySimulation : public Simulation {

using Simulation::Simulation; // making the constructors of the base class available

private:
    /**
     * calculate the force for all particles
     */
    virtual void calculateF();
};