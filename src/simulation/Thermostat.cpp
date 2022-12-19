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

Thermostat::Thermostat(std::shared_ptr<ParticleContainer> particleContainer, float initTemperature) {
    // Spdlog
    _logicLogger = spdlog::get("simulation_logger");
    _memoryLogger = spdlog::get("memory_logger");

    this->particleContainer = particleContainer;
    if (initTemperature < 0) {
        _logicLogger->info("Thermostat: Init temperature must be positive or zero!");
        _logicLogger->info("Thermostat: Using the absolute value of the provided init temperature!");
        initTemperature *= -1;
    }
    this->initTemperature = initTemperature;
    this->targetTemperature = initTemperature;
    this->temperatureDelta = -1;

    _memoryLogger->info("Thermostat generated!");
}

// Destructor

Thermostat::~Thermostat() {
    _memoryLogger->info("Thermostat destructed!");
}

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

void Thermostat::initializeBrownianMotion() {
    // dimensions
    int dimensions = 2;

    for (auto &p: particleContainer->getActiveParticles()) {
        float meanV = sqrt(this->initTemperature / p.getM());
        p.setV(p.getV() + maxwellBoltzmannDistributedVelocity(meanV, dimensions));
    }

    _logicLogger->info("Initialized brownian motion with initial temperature: {}", this->initTemperature);
}

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

// Getters

const float Thermostat::getTargetTemperature() {
    return this->targetTemperature;
}

const float Thermostat::getTemperatureDelta() {
    return this->temperatureDelta;
}

const float Thermostat::getInitTemperature() {
    return this->initTemperature;
}

// Setters

const void Thermostat::setTargetTemperature(float targetTemperature) {
    if (targetTemperature < 0) {
        _logicLogger->info("Thermostat: Target temperature must be positive or zero!");
        _logicLogger->info("Thermostat: Using the absolute value of the provided target temperature!");
        targetTemperature *= -1;
    }
    this->targetTemperature = targetTemperature;
}

const void Thermostat::setTemperatureDelta(float temperatureDelta) {
    if (temperatureDelta < 0) {
        _logicLogger->info("Thermostat: Delta temperature must be positive or zero!");
        _logicLogger->info("Thermostat: Using the absolute value of the provided delta temperature!");
        temperatureDelta *= -1;
    }
    this->temperatureDelta = temperatureDelta;
}