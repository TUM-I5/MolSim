//
// Created by Alp Kaan Aksu, Berke Saylan on 29.10.23.
//

#include "ParticleContainer.h"

#include "utils/Generator.h"

ParticleContainer::ParticleContainer() {
    particles = std::vector<Particle>();

}

/**
 * @brief Apply a function to all particles in the container
 * Useful for applying velocity, position etc. calculations on each particle in the container
 * @param function The function to be applied to each particle
 */
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

/**
 * @brief Apply a function to all unique pairs of particles in the container
 * Useful for utilizing Newton's third law for more efficient force calculation between particles
 * @param function The function to be applied to each pair of particles
 */
void ParticleContainer::applyToAllPairsOnce(const std::function<void(Particle&, Particle&)>& function) {
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
            function(particles[i], particles[j]);
            function(particles[j], particles[i]);
        }
    }
}

/**
 * @brief Add a single particle to the container
 *
 * @param particle The particle to be added
 */
void ParticleContainer::add(const Particle &particle) {
    particles.push_back(particle);
}

/**
 * @brief Add particles from a JSON configuration to the container
 *
 * @param objects The JSON configuration representing the particles
 */
void ParticleContainer::add(const json &objects) {
    for (auto &object : objects) {
        if (object["type"] == "particle") {
            add(Particle{object["position"], object["velocity"], object["mass"]});
        } else if (object["type"] == "cuboid") {
            Generator::cuboid(*this, object["position"], object["size"], object["mesh_width"], object["velocity"], object["mass"]);
        }
    }
}

/**
 * @brief Return the number of particles stored in the container
 */
int ParticleContainer::size() {
    return static_cast<int>(particles.size());
}

/**
 * @brief Remove a single particle from the container
 *
 * @param particle The particle to be removed
 */
void ParticleContainer::remove(Particle &particle) {
    for (size_t i = 0; i < particles.size(); ++i) {
        if (particles[i] == particle) {
            particles.erase(particles.begin() + i);
            break;
        }
    }
}