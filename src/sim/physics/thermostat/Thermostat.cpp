#include "Thermostat.h"
#include "data/ParticleContainer.h"
#include "data/Particle.h"

#include <vector>

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