#include "ParticleContainer.h"

#include "utils/ArrayUtils.h"

ParticleContainer::ParticleContainer(int n) { particles.reserve(n); }

void ParticleContainer::reserve(size_t n) { particles.reserve(n); }

std::size_t ParticleContainer::size() const { return particles.size(); }

std::size_t ParticleContainer::capacity() const { return particles.capacity(); }

void ParticleContainer::addParticle(const Particle& p) { particles.push_back(p); }

void ParticleContainer::addParticle(Particle&& p) { particles.push_back(std::move(p)); }

Particle& ParticleContainer::operator[](int i) { return particles[i]; }

std::vector<Particle>::iterator ParticleContainer::begin() { return particles.begin(); }

std::vector<Particle>::iterator ParticleContainer::end() { return particles.end(); }

std::vector<Particle>::const_iterator ParticleContainer::begin() const { return particles.begin(); }

std::vector<Particle>::const_iterator ParticleContainer::end() const { return particles.end(); }

void ParticleContainer::applyPairwiseForces(const std::vector<std::unique_ptr<ForceSource>>& force_sources) {
    for (auto it1 = particles.begin(); it1 != particles.end(); ++it1) {
        for (auto it2 = (it1 + 1); it2 != particles.end(); ++it2) {
            std::array<double, 3> total_force{0, 0, 0};
            for (auto& force : force_sources) {
                total_force = total_force + force->calculateForce(*it1, *it2);
            }
            it1->setF(it1->getF() + total_force);
            it2->setF(it2->getF() - total_force);
        }
    }
}
