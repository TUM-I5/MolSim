//
// Created by Feryal Ezgi on 9.12.2023.
//

#include "Thermostat.h"
#include <spdlog/spdlog.h>
#include <cmath>

// maxTemperatureChange set to default value zero makes sure that the scaling factor stays one
// and the thermostat is not applied, if it wasn't initialized correctly
Thermostat::Thermostat() : targetTemperature(0.0),
                           maxTemperatureChange(0.0),
                           thermostatInterval(0),
                           numDimensions(5),
                           initializeWithBrownianMotion(false) {
}

Thermostat::Thermostat(double initialTemperature, size_t thermostatInterval, size_t numDimensions,
                       bool initializeWithBrownianMotion)
        : initialTemperature(initialTemperature),
          targetTemperature(initialTemperature),
          maxTemperatureChange(std::numeric_limits<double>::infinity()),
          thermostatInterval(thermostatInterval),
          numDimensions(numDimensions),
          initializeWithBrownianMotion(initializeWithBrownianMotion) {
}


void Thermostat::scaleVelocities(ParticleContainer &particleContainer) {
    double currentTemperature = getCurrentTemperature(particleContainer);
    double temperatureChange = std::copysign(
            std::min(std::abs(targetTemperature - currentTemperature), maxTemperatureChange),
            targetTemperature - currentTemperature);

    double newTemperature = currentTemperature + temperatureChange;

    double scalingFactor = std::sqrt(newTemperature / currentTemperature);

    // Check for zero temperature to avoid division by zero
    if (currentTemperature == 0.0 && (std::isnan(scalingFactor) || std::isinf(scalingFactor))) {
        spdlog::warn("Current temperature is zero. Velocity scaling is skipped.");
        return;
    }

    particleContainer.applyToAll([&scalingFactor](Particle &p) {
        p.setV(scalingFactor * p.getV());
    });
}


double Thermostat::getCurrentTemperature(ParticleContainer &particleContainer) {
    double kineticEnergy = 0.0;
    particleContainer.applyToAll([&kineticEnergy](Particle &particle) {
        double vSquared = particle.getV()[0] * particle.getV()[0] + particle.getV()[1] * particle.getV()[1] +
                          particle.getV()[2] * particle.getV()[2];

        kineticEnergy = kineticEnergy + (0.5 * particle.getM() * vSquared);
    });


    // We assume everything to be dimensionless, therefore kB = 1.
    return 2 * kineticEnergy / (particleContainer.size() * numDimensions);
}


void Thermostat::initializeTemperature(ParticleContainer &particleContainer) {
    particleContainer.applyToAll([this](Particle &p) {
        auto v = p.getV();
        p.setV(p.getV() +
               maxwellBoltzmannDistributedVelocity(std::sqrt(initialTemperature / p.getM()), numDimensions));

    });

}


double Thermostat::getInitialTemperature() const {
    return initialTemperature;
}

void Thermostat::setInitialTemperature(double initialTemperature) {
    Thermostat::initialTemperature = initialTemperature;
}

double Thermostat::getTargetTemperature() const {
    return targetTemperature;
}

void Thermostat::setTargetTemperature(double targetTemperature) {
    Thermostat::targetTemperature = targetTemperature;
}

double Thermostat::getMaxTemperatureChange() const {
    return maxTemperatureChange;
}

void Thermostat::setMaxTemperatureChange(double maxTemperatureChange) {
    Thermostat::maxTemperatureChange = maxTemperatureChange;
}

size_t Thermostat::getThermostatInterval() const {
    return thermostatInterval;
}

void Thermostat::setThermostatInterval(size_t thermostatInterval) {
    Thermostat::thermostatInterval = thermostatInterval;
}

size_t Thermostat::getNumDimensions() const {
    return numDimensions;
}

void Thermostat::setNumDimensions(size_t numDimensions) {
    Thermostat::numDimensions = numDimensions;
}

bool Thermostat::isInitializeWithBrownianMotion() const {
    return initializeWithBrownianMotion;
}

void Thermostat::setInitializeWithBrownianMotion(bool initializeWithBrownianMotion) {
    Thermostat::initializeWithBrownianMotion = initializeWithBrownianMotion;
}
