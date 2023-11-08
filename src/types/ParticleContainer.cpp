#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(int n) { particles.reserve(n); }

void ParticleContainer::reserve(int n) { particles.reserve(n); }

std::size_t ParticleContainer::size() const { return particles.size(); }

std::size_t ParticleContainer::capacity() const { return particles.capacity(); }

void ParticleContainer::addParticle(const Particle& p) { particles.push_back(p); }

void ParticleContainer::addParticle(Particle&& p) { particles.push_back(std::move(p)); }

Particle& ParticleContainer::operator[](int i) { return particles[i]; }

std::vector<Particle>::iterator ParticleContainer::begin() { return particles.begin(); }

std::vector<Particle>::iterator ParticleContainer::end() { return particles.end(); }
