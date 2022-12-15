/*
 *  Thermostat.cpp
 *
 *  Created on: 14.12.2022
 *      Author: borisov
 */

#include "./Thermostat.h"
#include "../utils/ArrayUtils.h"
#include "../utils/MaxwellBoltzmannDistribution.h"

#include <iostream>

// Constructors

Thermostat::Thermostat(std::shared_ptr<ParticleContainer> particleContainer, float targetTemperature) {
    this->particleContainer = particleContainer;
    this->targetTemperature = targetTemperature;
    this->temperatureDelta = -1;
}

Thermostat::Thermostat(std::shared_ptr<ParticleContainer> particleContainer, float targetTemperature, float temperatureDelta) {
    this->particleContainer = particleContainer;
    this->targetTemperature = targetTemperature;
    this->temperatureDelta = temperatureDelta;
}

// public 

void Thermostat::apply() {
    float currentTemperature = calculateCurrentTemperature();
    float newTemperature = calculateNewTemperature(currentTemperature);

    // TODO: CurrentTemp = 0 and temp negative

    // calculate beta
    float beta = sqrt(newTemperature / currentTemperature);
    std::cout << "New Temp: " << newTemperature << std::endl;
    std::cout << "Current Temp: " << currentTemperature << std::endl;
    std::cout << "Beta: " << beta << std::endl;

    for (auto &p: particleContainer->getActiveParticles()) {
        p.setV(beta * p.getV());
    }
}

void Thermostat::initializeBrownianMotion(float tempInit) {
    // dimensions
    int dimensions = 2;

    for (auto &p: particleContainer->getActiveParticles()) {
        float meanV = sqrt(tempInit / p.getM());
        p.setV(p.getV() + maxwellBoltzmannDistributedVelocity(meanV, dimensions));
    }
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