/*
 * ProgramParameters.h
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#pragma once 


#include "../model/ParticleContainer.h"
#include "spdlog/sinks/basic_file_sink.h" 
#include "../inputReader/InputFacade.h"
#include "spdlog/spdlog.h"

#include <list>

/**
 * @brief wrapper for all program parameters. Makes it easy to safe and rerun with the same parameters
*/
class ProgramParameters{
private: 
    ParticleContainer _particleContainer; /// container for all the particles
    double _end_time; /// end_time of the simulation
    double _delta_t; /// increase in step size for the time
    bool _showMenu; /// specifies if the menu should be shown
    std::unique_ptr<InputFacade> _inputFacade; /// reads the input
    std::shared_ptr<spdlog::logger> _memoryLogger; /// a speedlog logger which logs construction and destruction of particles

public: 
    /**
     * @brief constructor for the ProgramParameters, initialises all of the parameters of the class
    */
    ProgramParameters(); 
    ~ProgramParameters(); 

    /**
     * @brief reads from an input file - either a cuboid or separate particles
     * @param filename the absolute or relative path to the filename
    */
    const void readFromFile(const char* filename); 

    /**
     * @brief runs the simulation with the parameters that are currently set
    */
    const void runWithCurrentParameters(); 

    /**
     * @brief removes all particles from the simulation
    */
    const void resetParameters(); 

    /**
     * Getters/Setters
    */
    const void setEndTime(double end_time); 

    const void setDeltaT(double delta_t); 

    const void setShowMenu(bool showMenu); 

    const bool getShowMenu() const; 

    ParticleContainer *getParticleContainer(); 

    const double getEndTime() const; 

    const double getDeltaT() const; 
};