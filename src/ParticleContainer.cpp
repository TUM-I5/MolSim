#include "ParticleContainer.h"

#include "utils/ArrayUtils.h"

ParticleContainer::ParticleContainer(int n) { particles.reserve(n); }

void ParticleContainer::reserve(int n) { particles.reserve(n); }

std::size_t ParticleContainer::size() { return particles.size(); }

void ParticleContainer::addParticle(Particle& p) { particles.push_back(p); }

void ParticleContainer::resetForces() {
    for (auto& p : particles) {
        p.setOldF(p.getF());
        p.setF({0, 0, 0});
    }
}

void ParticleContainer::applyForceSource(ForceSource& forceSource) {
    for (Particle& p : particles) {
        for (Particle& q : particles) {
            if (&p == &q) {
                continue;
            }

            auto force = forceSource.calculateForce(p, q);

            // add force to particle
            p.setF(p.getF() + force);
        }
    }
}

Particle& ParticleContainer::operator[](int i) { return particles[i]; }

ParticleContainer::Iterator ParticleContainer::begin() { return Iterator(&particles[0]); }

ParticleContainer::Iterator ParticleContainer::end() { return Iterator(&particles[particles.size()]); }
