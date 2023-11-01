//
// Created by Alp Kaan Aksu on 29.10.23.
//

#ifndef PSEMOLDYN_GROUPF_PARTICLECONTAINER_H
#define PSEMOLDYN_GROUPF_PARTICLECONTAINER_H

#include <vector>
#include "outputWriter/VTKWriter.h"

#include "models/Particle.h"

class ParticleContainer {
private:
    std::vector<Particle> particles;
public:
    ParticleContainer();

    void applyToAll(const std::function<void(Particle &)>& function);

    void applyToAllPairs(const std::function<void(Particle &, Particle &)> &function);

    void add(const Particle &particle);

    size_t count();

    void plot(outputWriter::VTKWriter &writer);
};


#endif //PSEMOLDYN_GROUPF_PARTICLECONTAINER_H
