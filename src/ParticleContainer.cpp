//
// Created by sophy on 02.11.2023.
//

#include "ParticleContainer.h"
#incude <list>

std::list<Particle> particles;

ParticleContainer::ParticleContainer() {
    particles = std::list<Particle>();
}

ParticleContainer::ParticleContainer(std::list<Particle> pList) {
    particles = pList;
}

std::list<Particle> getParticles() {
    return particles;
}

void addParticle(Particle &particle) {
    particles.push_back(particle);
}

int size() {
    return particles.size();
}



