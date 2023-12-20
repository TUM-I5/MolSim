//
// Created by Feryal Ezgi on 9.12.2023.
//
#pragma once

#include <limits>
#include "../models/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

class Thermostat {
private:
    double initialTemperature;
    double targetTemperature;
    double maxTemperatureChange;
    size_t thermostatInterval;
    size_t numDimensions;
    bool initializeWithBrownianMotion;

public:
    Thermostat();

    // Minimum requirement for the thermostat, ∆T = ∞ and targetTemperature = initialTemperature
    Thermostat(double initialTemperature, size_t thermostatInterval, size_t numDimensions,
               bool initializeWithBrownianMotion);

    void initializeTemperature(ParticleContainer &particleContainer);

    // Function to scale velocities directly or gradually
    void scaleVelocities(ParticleContainer &particleContainer);

    // Function to get the current temperature
    double getCurrentTemperature(ParticleContainer &particleContainer);

    // Function to calculate kinetic energy
    double calculateKineticEnergy(ParticleContainer &particleContainer);


    double getInitialTemperature() const;

    void setInitialTemperature(double initialTemperature);

    double getTargetTemperature() const;

    void setTargetTemperature(double targetTemperature);

    double getMaxTemperatureChange() const;

    void setMaxTemperatureChange(double maxTemperatureChange);

    size_t getThermostatInterval() const;

    void setThermostatInterval(size_t thermostatInterval);

    size_t getNumDimensions() const;

    void setNumDimensions(size_t numDimensions);

    bool isInitializeWithBrownianMotion() const;

    void setInitializeWithBrownianMotion(bool initializeWithBrownianMotion);

};


