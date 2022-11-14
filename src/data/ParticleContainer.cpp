#include "ParticleContainer.h"
#include "Particle.h"
#include <vector>

ParticleContainer::ParticleContainer() = default;

ParticleContainer::ParticleContainer(const std::vector<Particle>& buffer) {
    for (const auto& p : buffer) particles.emplace_back(p);
}

unsigned long ParticleContainer::size() {
    return particles.size();
}

std::vector<Particle> &ParticleContainer::getParticles() {
    return particles;
}

Particle &ParticleContainer::getParticle(unsigned long i) {
    if (i < particles.size()) {
        return particles[i];
    } else {
        throw std::runtime_error("Tried to access Particle with index out of bounds!\n");
    }
}

void ParticleContainer::forAllParticles(void (function)(Particle &p)) {
    std::for_each(particles.begin(), particles.end(), [&](Particle &g) { function(g); });
}

void ParticleContainer::forAllPairs(void (function)(Particle &p1, Particle &p2)) {
    for (u_int32_t i = 0; i < particles.size(); i++) {
        for (u_int32_t j = i + 1; j < particles.size(); j++) {
            Particle &p1 = particles[i];
            Particle &p2 = particles[j];
            function(p1, p2);
        }
    }
}

ParticleContainer::Iterator ParticleContainer::begin() {
    return Iterator{particles.begin()} ;
}

ParticleContainer::Iterator ParticleContainer::end() {
    return Iterator{particles.end()};
}

void ParticleContainer::addParticle(const Particle& p){
    particles.emplace_back(p);
}
