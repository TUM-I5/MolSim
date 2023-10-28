
#include "ParticleContainer.h"


ParticleContainer::ParticleContainer(std::list <Particle> part_list) : particles(part_list.begin(), part_list.end()) {}

ParticleContainer::ParticleContainer() : particles() {}

ParticleContainer::~ParticleContainer() {}

using iterator = std::vector<Particle>::iterator;
using const_iterator = std::vector<Particle>::const_iterator;

iterator ParticleContainer::begin() { return particles.begin(); }

iterator ParticleContainer::end() { return particles.end(); }


Particle &ParticleContainer::operator[](size_t i) {
    return particles[i];
}


size_t ParticleContainer::size() {
    return particles.size();
};
