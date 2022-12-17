/*
 * Simulation.h
 *
 * Created: 31.10.2022
 * Author:  wohlrapp
 */

#pragma once

#include "../model/ParticleContainer.h"
#include "../model/ProgramParameters.h"
#include "InterParticleForce.h"
#include "SingleParticleForce.h"
#include "spdlog/spdlog.h"

#include <memory>

/**
 * @brief Base class to abstract the simulation, provides shared calculation for the position and the velocity, the calculation of the force is abstract
 */
class Simulation
{

private:
    ProgramParameters *_programParameters;
    std::shared_ptr<InterParticleForce> _interParticleForceCalculation;
    std::shared_ptr<SingleParticleForce> _singleParticleForceCalculation; 
    std::shared_ptr<spdlog::logger> _logicLogger;
    /**
     * a speedlog logger which logs construction and destruction of particles
     */
    std::shared_ptr<spdlog::logger> _memoryLogger;

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
     * @param programParameters The container that wrapps all parameters used in the program
     */
    Simulation(ProgramParameters *programParameters);

    ~Simulation();

    /**
     * @brief Performs the simulation from 0 to end_time in delta_t increments
     */
    const void simulate();

    const std::shared_ptr<spdlog::logger> getLogicLogger() const;
};