//
// Created by sophy on 02.11.2023.
//

#include "ParticleContainer.h"
#include <list>

std::list<Particle> particles;

ParticleContainer::ParticleContainer() {
    particles = std::list<Particle>();
}

ParticleContainer::ParticleContainer(std::list<Particle> pList) {
    particles = pList;
}

std::list<Particle> ParticleContainer::getParticles() {
    return particles;
}

void ParticleContainer::addParticle(Particle &particle) {
    particles.push_back(particle);
}

int ParticleContainer::size() {
    return particles.size();
}



