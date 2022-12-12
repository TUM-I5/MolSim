#include "Thermostat.h"
#include "ParticleContainer.h"
#include "Particle.h"

#include <vector>

Thermostat::Thermostat(ParticleContainer& particleContainer, double T_t, double dT, unsigned int cT, unsigned int dimensions) : 
    pc{particleContainer}, deltaTemp{dT}, Ttarget{T_t}, countThreshold{cT}, dims{dimensions} {}

Thermostat::~Thermostat(){}

void Thermostat::setParticleContainer(ParticleContainer& particleContainer){
    this->pc = particleContainer;
}

double Thermostat::computeBeta(){
    double Tcur{computeCurrentTemp()};

    double Tnew;
    if(std::abs(Ttarget - Tcur) > deltaTemp){
        Tnew = Tcur + std::signbit(Ttarget-Tcur) * deltaTemp;
    }else{
        Tnew = this->Ttarget;
    }

    return std::sqrt(Tnew/Tcur);
}

void Thermostat::notify(){
    //TODO: depending on the answer (see discussion in discord) isActive needs to get deleted as a variable 
    //and the if(isActive)-part needs to be taken into the first if-statement
    //or this part is correct so far..
    countSinceLastActivation++;
    if(countSinceLastActivation >= countThreshold){
        countSinceLastActivation -= countThreshold;
        getCooking();
    }
}