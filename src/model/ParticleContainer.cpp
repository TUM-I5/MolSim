/*
 * ParticleContainer.cpp 
 * 
 * Created: 31.10.2022 
 * Author:  wohlrapp 
*/

#include "ParticleContainer.h"

const void ParticleContainer::iterateParticles(std::function<void(Particle&)> f) {
    for(Particle &p: particles){
        f(p);   
    }
}; 

const void ParticleContainer::iterateParticlePairs(std::function<void(Particle&, Particle&)> f) {
    // Since we use a vector we can directly access the particles through indexing
    // Because f_ij = -f_ji, we can save (n^2)/2 iterations by starting the inner loop at i+1
    for(long unsigned int i = 0; i < particles.size(); i++){
        for(long unsigned int j = i + 1; j < particles.size(); j++){
            f(particles[i], particles[j]);
        }
    }
} 

const int ParticleContainer::size() const {return particles.size(); }

const void ParticleContainer::resetParticles() {
    particles.clear(); 
} 

const void ParticleContainer::addParticle(std::array<double, 3> &x, std::array<double, 3> &v, double &m) {
    // using the std function to create the new particle so it does not need to be copied to the right memory address
    particles.emplace_back(x, v, m);
}

std::vector<Particle> &ParticleContainer::getParticles() { return particles; } 


