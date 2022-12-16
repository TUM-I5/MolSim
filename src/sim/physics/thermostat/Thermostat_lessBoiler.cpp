#include "Thermostat.h"
#include "data/ParticleContainer.h"
#include "data/Particle.h"

#include <vector>


#ifndef boil
void Thermostat::getCooking(){
    double beta{computeBeta()};

    //v = beta*v for all active Particles
    pc.runOnActiveData([&](std::vector<double> &force,
                                   std::vector<double> &oldForce,
                                   std::vector<double> &x,
                                   std::vector<double> &v,
                                   std::vector<double> &m,
                                   std::vector<int> &type,
                                   unsigned long count,
                                   std::vector<double> &eps,
                                   std::vector<double> &sig,
                                   std::vector<unsigned long> &activeParticles){
        for(auto a: activeParticles){
            v[3*a] = beta * v[3*a];
            v[3*a+1] = beta * v[3*a+1];
            v[3*a+2] = beta * v[3*a+2];
        }
    });
}


double Thermostat::computeCurrentTemp(){
    //E_kin = sum_particles (m* <v,v>/2)
    //E_kin = #dims*#particles*T/2

    //T = sum_particles(m*<v,v>)/(#dims*#particles)

    double sum{0};
    //pc.forAllParticles([&sum](Particle& p ){sum += p.getM() * (p.getX().dot(p.getX()));});
    pc.runOnActiveData([&sum](std::vector<double> &force,
                                std::vector<double> &oldForce,
                                std::vector<double> &x,
                                std::vector<double> &v,
                                std::vector<double> &m,
                                std::vector<int> &type,
                                unsigned long count,
                                std::vector<double> &eps,
                                std::vector<double> &sig,
                                std::vector<unsigned long> &activeParticles){
    for(auto a: activeParticles){
        sum += m[a] * (v[3*a]*v[3*a] + v[3*a+1]*v[3*a+1] + v[3*a+2]*v[3*a+2]);
    }
    });
    return sum/(dims*pc.activeSize());
}
#endif
