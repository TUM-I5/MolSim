#pragma once

#include "Particle.h"
#include <vector>
#include <list>

class ParticleContainer {


    private:
    std::vector<Particle> particles;

    public:

    using iterator = std::vector<Particle>::iterator ;
    using const_iterator = std::vector<Particle>::const_iterator;

    iterator begin();

    iterator end();

    ParticleContainer();
    ParticleContainer(std::list<Particle>);

    virtual ~ParticleContainer();

    Particle& operator[](size_t);

    size_t size();

};


