#pragma once

#include "ParticleContainer.h"
#include "Particle.h"

#include <vector>

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
    /**isActive indicates whether the Thermostat is currently just counting or actually changing the temperature*/
    bool isActive{false};
    /**Thermostat needs to know the number of dimenions for the temperature-computation*/
    unsigned int dims;
    /**Target temperature that the Thermostat tries to reach*/
    double Ttarget;
    /**determines the maximum temperature manipulation in one iteration step*/
    double deltaTemp;


public:
    Thermostat(ParticleContainer& pc, double Ttarget, double deltaTemp, unsigned int countThreshhold = 100, unsigned int dims = 2);

    ~Thermostat();

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

