/*
 *  Thermostat.h
 *
 *  Created on: 14.12.2022
 *      Author: borisov
 */

#pragma once

#include "../model/Particle.h"
#include "../model/ParticleContainer.h"
#include "spdlog/spdlog.h"

/**
 * @brief class that regulates the temperature within a system containing particles
 */
class Thermostat {
private:
    // desired temperature in wich the system wants to be
    float targetTemperature;

    // maximum absolute temperature change allowed for one application of the thermostat
    float temperatureDelta;

    // particles of the observed system
    std::shared_ptr<ParticleContainer> particleContainer;
    
    // a speedlog logger which logs the logic flow of the simulation
    std::shared_ptr<spdlog::logger> _logicLogger;
    
    //a speedlog logger which logs construction and destruction of particles
    std::shared_ptr<spdlog::logger> _memoryLogger;

public:
    /**
     * @brief calculates the current Temperature of the system (using the kinetic energy)
     * 
     * @return float the current temperature of the system
     */
    float calculateCurrentTemperature();

    /**
     * @brief calculates the new temperature of a system, according to targetTemperature and temperatureDelta
     * 
     * @param currentTemperature current temperature in the system
     * @return float the new temperature the system should have after the thermostat is applied
     */
    float calculateNewTemperature(float currentTemperature);

    ~Thermostat();

    /**
     * @brief Construct a new Thermostat object
     * 
     * @param particleContainer particles of the observed system
     * @param targetTemperature target temperature of the system the thermostat is watching
     */
    Thermostat(std::shared_ptr<ParticleContainer> particleContainer, float targetTemperature);

    /**
     * @brief Construct a new Thermostat object
     * 
     * @param particleContainer particles of the observed system
     * @param targetTemperature target temperature of the system the thermostat is watching
     * @param temperatureDelta maximum absolute temperature change allowed for one application of the thermostat
     */
    Thermostat(std::shared_ptr<ParticleContainer> particleContainer, float targetTemperature, float temperatureDelta);

    /**
     * @brief changes the temperature of the system towards the targetTemperature
     */
    void apply();

    /**
     * @brief initializes the velocity of the particles with the brownian motion
     * 
     * @param tempInit the initial temperature
     */
    void initializeBrownianMotion(float tempInit);
};