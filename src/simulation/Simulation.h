/*
 * Simulation.h 
 * 
 * Created: 31.10.2022 
 * Author:  wohlrapp 
*/

#pragma once

#include "../model/ParticleContainer.h"

/** 
 * @brief Base class for the simulation, abstracting the simulation 
*/
class Simulation{

private: 
    ParticleContainer particleContainer; /// wrapper for the particles used in the simulation
    double end_time; /// specifies the end of the simulation
    double delta_t; /// specifies the time increments

    /**
     * calculate the position for all particles
     */
    void calculateX();

    /**
     * calculate the force for all particles, this function is abstract and must be implemented by all derived classes
     */
    virtual void calculateF() = 0;

    /**
     * calculate the position for all particles
     */
    void calculateV();


public: 
    /**
     * @param ParticleContainer &particleContainer_arg, double end_time_arg, double delta_t_arg
    */
    Simulation(ParticleContainer &particleContainer_arg, double end_time_arg, double delta_t_arg); 

    /**
     * performs the simulation from 0 to end_time in delta_t increments
    */
    const void simulate();

    ParticleContainer &getParticleContainer(); 

    const double &getEndTime() const; 

    const double &getDeltaT() const; 
};