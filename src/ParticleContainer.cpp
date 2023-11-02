//
// Created by sophy on 02.11.2023.
//

#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(std::list <Particle> particle_list) : particles(part_list.begin(), part_list.end()) {}


ParticleContainer::ParticleContainer(std::vector <Particle> particles) {
    ParticleContainer::particles = particles;
}

void addParticle(const Particle &particle) {
    particles.push_back(particle);
}

using iterator = std::list<Particle>::iterator;

iterator begin() {
    return particles.begin();
}

iterator end() {
    return particles.end();
}



