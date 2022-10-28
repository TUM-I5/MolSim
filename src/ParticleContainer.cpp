#include "ParticleContainer.h"
#include "Particle.h"
#include "FileReader.h"
#include <vector>

ParticleContainer::ParticleContainer(){
    }

ParticleContainer::ParticleContainer(char* filename){
    FileReader fileReader;
    fileReader.readFile(particles, filename);
    }

int ParticleContainer::size(){
    return particles.size();
}

std::vector<Particle>& ParticleContainer::getParticles(){
    return particles;
}

Particle& ParticleContainer::getParticle(int i){
    if(0<=i && i<particles.size()){
        return particles[i];
    }
    else{
        throw std::runtime_error("Tried to access Particle with index out of bounds!\n");
    }
}

void ParticleContainer::forAllParticles(void (function)(Particle& p)){
    std::for_each(particles.begin(), particles.end(), [&](Particle &g) {function(g);});
}

void ParticleContainer::forAllPairs(void (function)(Particle& p1, Particle& p2)){
    for (u_int32_t i = 0; i < particles.size(); i++) {
        for (u_int32_t j = i + 1; j < particles.size(); j++) {
            Particle &p1 = particles[i];
            Particle &p2 = particles[j];
            function(p1, p2);
        }
    }
}
