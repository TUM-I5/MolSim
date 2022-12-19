/*
 *  Thermostat.cpp
 *
 *  Created on: 14.12.2022
 *      Author: borisov
 */

#include "./Thermostat.h"
#include "../utils/ArrayUtils.h"
#include "../utils/MaxwellBoltzmannDistribution.h"


// Constructors

Thermostat::Thermostat(std::shared_ptr<ParticleContainer> particleContainer, float targetTemperature) {
    this->particleContainer = particleContainer;
    if (targetTemperature < 0) {
        _logicLogger->info("Thermostat: Target temperature must be positive or zero!");
        _logicLogger->info("Thermostat: Using the absolute value of the provided target temperature!");
        targetTemperature *= -1;
    }
    this->targetTemperature = targetTemperature;
    this->temperatureDelta = -1;

    // Spdlog
    _logicLogger = spdlog::get("simulation_logger");
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("Thermostat generated!");
}

Thermostat::Thermostat(std::shared_ptr<ParticleContainer> particleContainer, float targetTemperature, float temperatureDelta) {
    this->particleContainer = particleContainer;
    if (targetTemperature < 0) {
        _logicLogger->info("Thermostat: Target temperature must be positive or zero!");
        _logicLogger->info("Thermostat: Using the absolute value of the provided target temperature!");
        targetTemperature *= -1;
    }
    this->targetTemperature = targetTemperature;
    if (temperatureDelta < 0) {
        _logicLogger->info("Thermostat: Delta temperature must be positive or zero!");
        _logicLogger->info("Thermostat: Using the absolute value of the provided delta temperature!");
        temperatureDelta *= -1;
    }
    this->temperatureDelta = temperatureDelta;

    // Spdlog
    _logicLogger = spdlog::get("simulation_logger");
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("Thermostat generated!");
}

// Destructor

Thermostat::~Thermostat() {
    _memoryLogger->info("Thermostat destructed!");
}

// public 

void Thermostat::apply() {
    float currentTemperature = calculateCurrentTemperature();
    float newTemperature = calculateNewTemperature(currentTemperature);

    if (currentTemperature == 0) {
        return;
    }

    // calculate beta
    float beta = sqrt(newTemperature / currentTemperature);

    for (auto &p: particleContainer->getActiveParticles()) {
        p.setV(beta * p.getV());
    }

    _logicLogger->info("Temperature set to {}", newTemperature);
}

void Thermostat::initializeBrownianMotion(float tempInit) {
    // dimensions
    int dimensions = 2;

    for (auto &p: particleContainer->getActiveParticles()) {
        float meanV = sqrt(tempInit / p.getM());
        p.setV(p.getV() + maxwellBoltzmannDistributedVelocity(meanV, dimensions));
    }

    _logicLogger->info("Initialized brownian motion with initial temperature: {}", tempInit);
}

// private

float Thermostat::calculateCurrentTemperature() {
    // dimensions
    int dimensions = 2;

    // calculate the kinetic energie
    float kineticE = 0;

    if (particleContainer->size() == 0) {
        return 0;
    }

    for (auto &p: particleContainer->getActiveParticles()) {
        float dotProduct = pow(ArrayUtils::L2Norm(p.getV()), 2);
        kineticE += (p.getM() * dotProduct) / 2;
    }

    // calculate temperature from kinetic energie
    float temperature = (2 * kineticE) / (dimensions * particleContainer->size());

    _logicLogger->debug("Current temperature: {}", temperature);

    return temperature;
}

float Thermostat::calculateNewTemperature(float currentTemperature) {
    // no deltaTemperature set
    if (temperatureDelta == -1) {
        return targetTemperature;
    }

    // deltaTemperature set

    // deltaTemperature is bigger than what needs to be changed
    if (abs(targetTemperature - currentTemperature) <= temperatureDelta) {
        return targetTemperature;
    }

    // current temperature is too high
    if (currentTemperature > targetTemperature) {
        return currentTemperature - temperatureDelta;
    }
    // current temperature is too low
    if (currentTemperature < targetTemperature) {
        return currentTemperature + temperatureDelta;
    }
    // curr == targetTemperature
    return currentTemperature;
}