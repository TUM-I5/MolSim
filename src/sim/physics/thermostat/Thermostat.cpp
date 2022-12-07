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

double Thermostat::computeCurrentTemp(){
    //E_kin = sum_particles (m* <v,v>/2)
    //E_kin = #dims*#particles*T/2

    //T = sum_particles(m*<v,v>)/(#dims*#particles)

    //TODO implementation that is actually performant 

    double sum{0};
    pc.forAllParticles([&sum](Particle& p ){sum += p.getM() * (p.getX().dot(p.getX()));});
    return sum/(dims*pc.activeSize());
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