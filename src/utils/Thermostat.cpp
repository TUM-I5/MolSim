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


double Thermostat::calculateKineticEnergy(ParticleContainer &particleContainer) {
    double totalEnergy = 0.0;
    for (auto &particle: particleContainer.getParticles()) {
        double vSquared = particle.getV()[0] * particle.getV()[0] + particle.getV()[1] * particle.getV()[1] +
                          particle.getV()[2] * particle.getV()[2];
        totalEnergy += 0.5 * particle.getM() * vSquared;
    }
    return totalEnergy;
}

double Thermostat::calculateScalingFactor(ParticleContainer &particleContainer) {
    double currentTemperature = getCurrentTemperature(particleContainer);
    double temperatureChange = std::copysign(
            std::min(std::abs(targetTemperature - currentTemperature), maxTemperatureChange),
            targetTemperature - currentTemperature);

    double newTemperature = currentTemperature + temperatureChange;

    // Log a warning if the temperature ratio is less than zero
    if (newTemperature / currentTemperature < 0) {
        spdlog::warn("T_new to T_current ratio is less than zero! The scaling factor will be NaN.");
    }

    double scalingFactor = std::sqrt(newTemperature / currentTemperature);
    return scalingFactor;
}

void Thermostat::scaleVelocities(ParticleContainer &particleContainer) {
    particleContainer.applyToAll([&particleContainer, this](Particle &p) {
        p.setV(calculateScalingFactor(particleContainer) * p.getV());
    });
}

double Thermostat::getCurrentTemperature(ParticleContainer &particleContainer) {
    // We assume everything to be dimensionless, therefore kB = 1.
    double kineticEnergy = calculateKineticEnergy(particleContainer);
    return 2 * kineticEnergy / (particleContainer.size() * numDimensions);
}


void Thermostat::initializeTemperature(ParticleContainer &particleContainer) {
    particleContainer.applyToAll([this](Particle &p) {
        auto v = p.getV();
        if (v[0] == 0 && v[1] == 0 && v[2] == 0) {
            p.setV(p.getV() +
                   maxwellBoltzmannDistributedVelocity(std::sqrt(initialTemperature / p.getM()), numDimensions));
        }
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
