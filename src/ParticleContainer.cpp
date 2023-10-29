//
// Created by Berke Saylan, Alp Kaan Aksu on 29.10.23.
//

#include "ParticleContainer.h"
#include "outputWriter/VTKWriter.h"

ParticleContainer::ParticleContainer(const std::vector<Particle> &initialParticles) {
    particles = initialParticles;
};

const std::vector<Particle> &ParticleContainer::getParticles() const { return particles; }

void ParticleContainer::setParticles(const std::vector<Particle> &newParticles) {
    particles = newParticles;
}

void ParticleContainer::applyToAll(const std::function<void(Particle&)>& function) {
    for (auto &p: particles) {
        function(p);
    }
}

void ParticleContainer::applyToAllPairs(const std::function<void(Particle&, Particle&)>& function) {
    for (auto &p1: particles) {
        for (auto &p2 : particles) {
            if (p1 == p2) {
                // same particle. skip.
                continue;
            }

            function(p1, p2);
        }
    }
}

void ParticleContainer::plot(outputWriter::VTKWriter &writer) {
    for (auto &p: particles) {
        writer.plotParticle(p);
    }
}

void ParticleContainer::add(const Particle &particle) {
    particles.push_back(particle);
}

void ParticleContainer::remove(Particle &particle) {
    for (size_t i = 0; i < particles.size(); ++i) {
        if (particles[i] == particle) {
            particles.erase(particles.begin() + i);
            break;
        }
    }
}

size_t ParticleContainer::count() {
    return particles.size();
}






