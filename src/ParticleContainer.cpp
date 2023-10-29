#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(int n) { particles.reserve(n); }

void ParticleContainer::reserve(int n) { particles.reserve(n); }

std::size_t ParticleContainer::size() { return particles.size(); }

void ParticleContainer::addParticle(Particle &p) { particles.push_back(p); }

void ParticleContainer::setAndClearForces() {
    for (auto &p : particles) {
        p.setOldF(p.getF());
        p.setF({0, 0, 0});
    }
}

void ParticleContainer::applyForce(ForceCalculation &forceCalculation) {
    for (Particle &p : particles) {
        for (Particle &q : particles) {
            if (&p == &q) {
                continue;
            }
            forceCalculation.addForce(p, q);
        }
    }
}

Particle &ParticleContainer::operator[](int i) { return particles[i]; }

ParticleContainer::Iterator ParticleContainer::begin() { return Iterator(&particles[0]); }

ParticleContainer::Iterator ParticleContainer::end() { return Iterator(&particles[particles.size()]); }
