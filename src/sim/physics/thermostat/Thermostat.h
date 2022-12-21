#pragma once

#include "data/Particle.h"
#include "data/ParticleContainer.h"
#include "defaults.h"
#include "io/output/Logging.h"

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
    /**
     * Initializes Thermostat sets the current Temperature to TInit
     * In case the TInit is already surpassed from the user input nothing happens
     * If no particle is initialized with "ordered" movement the current temperature gets set to TInit exactely
     * If particles have "ordered" movement already every Body gets set to (approximately) TInit in their respective system
     * Therefore in this case THE CURRENT TEMPERATURE IS UNEQUAL TO TInit!
     *
     * This might be confusing but hopefully as close as possible to the intended behaviour.
     */
    explicit Thermostat(ParticleContainer& particleContainer, double T_t = default_t_target, unsigned int cT = default_n_term, unsigned int dimensions = default_dims, double dT = default_delta_temp, double TInit = default_t_init, bool thermoEnable = default_therm):
        pc(particleContainer), countThreshold(cT), dims(dimensions) {
        if(thermoEnable) {
            //if currentTemp != 0 and TempToCreate != 0 this WILL distort the ordered movement and initialize the temperature a bit
            //inhomogenous
            auto curTemp{computeCurrentTemp()};
            if(curTemp > TInit){
                io::output::loggers::general->info("Since user defined Temperature is lower than user defined \"ordered\" movement no additional heat is generated");
                return ;
            }
            auto TempToCreate{TInit - curTemp};
            if (TempToCreate != 0.) { initializeBrownTemp(TempToCreate); }

                //normalize towards our intended temperature:
                //save real input parameters
                auto realDeltaTemp{dT};
                auto realTtarget{T_t};

                //abuse getCooking to normalize to intended TInit
                this->Ttarget = TInit;
                this->deltaTemp = std::numeric_limits<double>::infinity();

                this->getCooking();

                //set values to their intended parameters
                deltaTemp = realDeltaTemp;
                Ttarget = realTtarget;
            }

        }


    ~Thermostat() = default;

    void setParticleContainer(ParticleContainer& pc);

    /**
     * computes the current temperature relative to the Coordinate System of the Simulation
     * @return
     */
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
     * @brief This method does the actual heat-manipulation.
     * An alternative name for this method would be adjustTemp()
     * 
     */
    void getCooking();

    /**
     * @brief adds brownian motion with f_i = std::sqrt(TInit/m) to each particle
     * 
     * @param TInit 
     */
    void initializeBrownTemp(double TInit);
};

