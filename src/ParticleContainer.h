//
// Created by sophy on 02.11.2023.
//

#ifndef PSEMOLDYN_GROUPE_PARTICLECONTAINER_H
#define PSEMOLDYN_GROUPE_PARTICLECONTAINER_H

#endif //PSEMOLDYN_GROUPE_PARTICLECONTAINER_H

#pragma once

#include <list>
#include "Particle.h"
#include <vector>
#include <utility>

class ParticleContainer {

private:

    /**
   * All the particles
   */
    std::vector <Particle> particles;

    /**
  * Pairs of all particles
  * a.e. for 4 particles p1,p2,p3,p4, there are 6 particle pairs: (p1,p2),(p1,p3),(p1,p4),(p2,p3),(p2,p4),(p3,p4).
  */
    std::vector<std::pair<Particle, Particle>> particlePairs;

public:

    ParticleContainer();

    ParticleContainer(std::vector <Particle> pList);

    void addParticle(Particle &particle);

    std::vector <Particle> getParticles();

    int size();

    void createParticlePairs();
};