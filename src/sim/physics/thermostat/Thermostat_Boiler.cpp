#include "Thermostat.h"
#include "data/ParticleContainer.h"
#include "data/Particle.h"

#include <vector>


void Thermostat::getCooking(){
    double beta{computeBeta()};
    //TODO implementation that is actually performant
    pc.forAllParticles([&](Particle&p){
        p.setV(beta*p.getV());
    });
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