//
// Created by sophy on 02.11.2023.
//

#ifndef PSEMOLDYN_GROUPE_PARTICLECONTAINER_H
#define PSEMOLDYN_GROUPE_PARTICLECONTAINER_H

#endif //PSEMOLDYN_GROUPE_PARTICLECONTAINER_H

#pragma once

#include <list>
#include "Particle.h"

class ParticleContainer {

private:

    std::list <Particle> particles;

public:

    ParticleContainer();

    ParticleContainer(std::list <Particle> pList);

    void addParticle(Particle &particle);

    std::list <Particle> getParticles();

    int size();
};