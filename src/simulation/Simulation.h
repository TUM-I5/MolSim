/*
 * Simulation.h 
 * 
 * Created: 31.10.2022 
 * Author:  wohlrapp 
*/

#pragma once

#include "../model/ParticleContainer.h"
#include "../model/ProgramParameters.h"
#include "ForceCalculation.h"
#include "spdlog/spdlog.h"

#include <memory>

/** 
 * @brief Base class to abstract the simulation, provides shared calculation for the position and the velocity, the calculation of the force is abstract
*/
class Simulation{

private: 
    ProgramParameters *_programParameters; 
    ForceCalculation *_forceCalculation; 
    std::shared_ptr<spdlog::logger> _logicLogger;


    /**
     * @brief Calculate the position for all particles
     */
    void calculateX();

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
    Simulation(ProgramParameters *programParameters); 

    /**
     * @brief Performs the simulation from 0 to end_time in delta_t increments
    */
    const void simulate();

    const std::shared_ptr<spdlog::logger> getLogicLogger() const;


};