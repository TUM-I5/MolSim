
#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() : particles() {}



ParticleContainer::~ParticleContainer() {}
/**
 *
 * @brief provides particle pairs based on a "triangle" on the matrix of
 * all possible pair combinations, to avoid redundant calls using F_ij = -F_ji
 *
 * sets next pair of particles to calculate the force in between of them or
 * to a pair of nullpointers to indicate the end
 *
 * @param std::pair<Particle*, Particle*> *pair to set the next pair of pointers into
 * @return None
 *
 */
void ParticleContainer::setNextPair(std::pair<Particle*, Particle*> &pair) {
    static size_t row = 0;
    static size_t column = 1;

    if(column < particles.size()) {
        pair.first = &particles[row];
        pair.second = &particles[column];
        column++;

    } else {
        row++;
       if(row < particles.size() - 1) {
           column = row + 1;
           pair.first = &particles[row];
           pair.second = &particles[column];
           column++;

       } else {
           row = 0;
           column = 1;
           pair.first = nullptr;
           pair.second = nullptr;
       }
    }
}

Particle* ParticleContainer::getNextParticle() {
    static size_t count = 0;

    if(count < particles.size()) {
        return &particles[count++];
    }

    count = 0;
    return nullptr;
}

void ParticleContainer::addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg) {
    particles.emplace_back(x_arg, v_arg, m_arg);
}


Particle &ParticleContainer::operator[](size_t i) {
    return particles[i];
}

void ParticleContainer::plotParticles(outputWriter::VTKWriter &writer) {
    for (auto & particle : particles) {
        writer.plotParticle(particle);
    }
}

void ParticleContainer::printParticles() {
    for (auto & particle : particles) {
        std::cout << particle << std::endl;
    }
}

size_t ParticleContainer::size() const {
    return particles.size();
};
