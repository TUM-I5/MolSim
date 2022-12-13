#pragma once

#include "data/Particle.h"
#include "data/ParticleContainer.h"

#include <vector>
#include <numeric>

/**
 * @brief Thermostat is responsible for adjusting the temperature and determining whether temp-adjustment is wanted
 * 
 */
class Thermostat
{
private:
    /**reference to the current ParticleContainer; the reference is needed to Compute the current temperature, manipulate the particles, etc */
    ParticleContainer& pc;  
    /**variable, that gets increased in every simulation step. If it is large enough the Thermostat starts with the temperature-regulation process */
    unsigned int countSinceLastActivation{0};
    /**amount of iterations that have to pass before the Thermostat starts doing his work*/
    unsigned int countThreshold;
    /**Thermostat needs to know the number of dimenions for the temperature-computation*/
    unsigned int dims;
    /**Target temperature that the Thermostat tries to reach*/
    double Ttarget;
    /**determines the maximum temperature manipulation in one iteration step*/
    double deltaTemp;


public:
    Thermostat(ParticleContainer& particleContainer, double T_t, unsigned int cT = 100, unsigned int dimensions = 2, double dT = std::numeric_limits<double>::infinity()):
        pc(particleContainer), countThreshold(cT), dims(dimensions), Ttarget(T_t), deltaTemp(dT)  {}

    ~Thermostat();

    Thermostat();

    void setParticleContainer(ParticleContainer& pc);

    double computeCurrentTemp();

    /**
     * @brief computes the Beta that should be applied
     * If T_target is not within reach, beta will depend on T_current and Delta_T
     * If T_target is withing reach, beta will depend on T_current and T_target
     * 
     * @return double 
     */
    double computeBeta();

    /**
     * @brief Notifies thermostat that a new iteration is being processed
     * Gets called in every iteration
     * 
     */
    void notify();

    /**
     * @brief This method does the actual heat-manipulation
     * 
     */
    void getCooking();
};

