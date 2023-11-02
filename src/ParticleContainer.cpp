
#include "ParticleContainer.h"


ParticleContainer::ParticleContainer(const std::list <Particle>& part_list) : amountParticles(part_list.size()) {
    //todo avoid remaining particle copies
    for(const Particle& particle : part_list) {
        particles.push_back(particle);
    }
}

ParticleContainer::ParticleContainer() : particles(), amountParticles() {}

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
    static int row = 0;
    static int column = 1;

    if(column < amountParticles) {
        pair.first = &particles[row];
        pair.second = &particles[column];
        column++;

    } else {
        row++;
       if(row < amountParticles - 1) {
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
    static int count = 0;

    if(count < amountParticles) {
        return &particles[count++];
    }

    count = 0;
    return nullptr;
}

void ParticleContainer::plotParticles(outputWriter::VTKWriter &writer) {
    for (int i = 0; i < amountParticles; i++) {
        writer.plotParticle(particles[i]);
    }
}

void ParticleContainer::printParticles() {
    for (int i = 0; i < amountParticles; i++) {
        std::cout << particles[i] << std::endl;
    }
}

size_t ParticleContainer::size() const {
    return amountParticles;
};
