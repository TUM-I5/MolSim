#pragma once

#include "Particle.h"
#include <vector>
#include <list>

class ParticleContainer {


private:
    /**
     * internal Data structure for storing particles(can be changed if interfaces of this class
     * are changed accordingly)
     */
    std::vector <Particle> particles;

public:

    /**
     * provide iterator interface
     */
    using iterator = std::vector<Particle>::iterator;
    using const_iterator = std::vector<Particle>::const_iterator;

    iterator begin();

    iterator end();


    /**
     * Constructor and Destructor
     */
    ParticleContainer();

    ParticleContainer(std::list <Particle>);

    virtual ~ParticleContainer();

    /**
     * Functions acting on ParticleContainer
     */

    Particle &operator[](size_t);

    size_t size();

};


