//
// Created by sophy on 02.11.2023.
//

#include "ParticleContainer.h"
#include <list>
#include <vector>
#include <utility> //for pair
#include <algorithm>
#include <cstddef>



std::vector <Particle> particleList;
std::vector <std::pair<Particle, Particle>> particlePairs;

ParticleContainer::ParticleContainer() {
    particleList = std::vector<Particle>();
}

ParticleContainer::ParticleContainer(std::vector <Particle> pVector) {
    particleList = pVector;
}


std::vector <Particle> ParticleContainer::getParticles() {
    return particleList;
}

void ParticleContainer::addParticle(Particle &particle) {
    particleList.push_back(particle);
}


void createParticlePairs() {
    for (auto &p1: particleList) {
        for (auto &p2: particleList) {
            if (&p1 != &p2) {
                std::pair <Particle, Particle> currentPair = std::make_pair(p1, p2);
                std::pair <Particle, Particle> reversePair = std::make_pair(p2, p1);

                // Check if the current pair or its reverse is already in particlePairs
                if (std::find(particlePairs.begin(), particlePairs.end(), currentPair) == particlePairs.end() &&
                    std::find(particlePairs.begin(), particlePairs.end(), reversePair) == particlePairs.end()) {
                    // Neither the current pair nor its reverse is in particlePairs, so add the current pair
                    particlePairs.push_back(currentPair);
                }
            }
        }
    }
}

std::size_t ParticleContainer::size() const {
    return particleList.size();
}



