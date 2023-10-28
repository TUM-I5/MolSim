#include "ParticleContainer.h"

ParticleContainer::Iterator ParticleContainer::begin() { return Iterator(&particles[0]); }

ParticleContainer::Iterator ParticleContainer::end() { return Iterator(&particles[particles.size()]); }

ParticleContainer::ParticleContainer(int n) { particles.reserve(n); }

void ParticleContainer::reserve(int n) { particles.reserve(n); }

std::size_t ParticleContainer::size() { return particles.size(); }

void ParticleContainer::addParticle(Particle &p) { particles.push_back(p); }

Particle &ParticleContainer::operator[](int i) { return particles[i]; }
