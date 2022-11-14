/*
 * Simulation.h 
 * 
 * Created: 31.10.2022 
 * Author:  wohlrapp 
*/

#pragma once

#include "../model/ParticleContainer.h"
#include "spdlog/spdlog.h"

#include <memory>

/** 
 * @brief Base class to abstract the simulation, provides shared calculation for the position and the velocity, the calculation of the force is abstract
*/
class Simulation{

private: 
    ParticleContainer *particleContainer; /// wrapper for the particles used in the simulation
    double end_time; /// specifies the end of the simulation
    double delta_t; /// specifies the time increments
    /**
     * A spdlog logger, which logs the logic of the program flow
     */
    std::shared_ptr<spdlog::logger> _logicLogger;


    /**
     * @brief Calculate the position for all particles
     */
    void calculateX();

    /**
     * @brief Calculate the force for all particles, this function is abstract and must be implemented by all derived classes
     */
    virtual void calculateF() = 0;

    /**
     * @brief calculate the position for all particles
     */
    void calculateV();


public: 
    /**
     * @param particleContainer_arg The container that wrapps around all particles 
     * @param end_time_arg The time when the simulation ends 
     * @param delta_t_arg The increase in time
     * @note The particlesContainer must already have the particles added
    */
    Simulation(ParticleContainer *particleContainer_arg, double end_time_arg, double delta_t_arg); 

    /**
     * @brief Performs the simulation from 0 to end_time in delta_t increments
    */
    const void simulate();

    ParticleContainer *getParticleContainer(); 

    const double &getEndTime() const; 

    const double &getDeltaT() const; 

    const std::shared_ptr<spdlog::logger> getLogicLogger() const;
};