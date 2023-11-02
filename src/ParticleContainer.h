//
// Created by sophy on 02.11.2023.
//

#ifndef PSEMOLDYN_GROUPE_PARTICLECONTAINER_H
#define PSEMOLDYN_GROUPE_PARTICLECONTAINER_H

#endif //PSEMOLDYN_GROUPE_PARTICLECONTAINER_H

#pragma once

class ParticleContainer {

public:

    ParticleContainer(std::list <Particle> particle_list)

    void addParticle(const Particle &particle) {
    }

    using iterator = std::list<Particle>::iterator;

    iterator begin() {
    }

    iterator end() {
    }
};