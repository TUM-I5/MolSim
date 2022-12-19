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

    // init temperature of the system
    float initTemperature;

    // particles of the observed system
    std::shared_ptr<ParticleContainer> particleContainer;
    
    // a speedlog logger which logs the logic flow of the simulation
    std::shared_ptr<spdlog::logger> _logicLogger;
    
    //a speedlog logger which logs construction and destruction of particles
    std::shared_ptr<spdlog::logger> _memoryLogger;

public:

    /**
     * @brief Construct a new Thermostat object
     * 
     * @param particleContainer particles of the observed system
     * @param initTemperature init temperature of the system the thermostat is watching
     */
    Thermostat(std::shared_ptr<ParticleContainer> particleContainer, float initTemperature);

    ~Thermostat();
    
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

    /**
     * @brief changes the temperature of the system towards the targetTemperature
     */
    void apply();

    /**
     * @brief initializes the velocity of the particles with the brownian motion
     */
    void initializeBrownianMotion();

    // Getters

    const float getTargetTemperature();

    const float getTemperatureDelta();

    const float getInitTemperature();

    // Setters

    const void setTargetTemperature(float targetTemperature);

    const void setTemperatureDelta(float temperatureDelta);
};