//
// Created by Berke Saylan, Alp Kaan Aksu on 29.10.2023.
//

#ifndef PSEMOLDYN_GROUPF_PARTICLECONTAINER_H
#define PSEMOLDYN_GROUPF_PARTICLECONTAINER_H

#include <vector>
#include "Particle.h"
#include "outputWriter/VTKWriter.h"
#include <functional>

class ParticleContainer {
private:

    std::vector<Particle> particles;

public:
    explicit ParticleContainer(const std::vector<Particle> &initialParticles = {});

    void add(const Particle &particle);

    size_t count();

    void plot(outputWriter::VTKWriter &writer);

    void applyToAll(const std::function<void(Particle &)> &function);

    void applyToAllPairs(const std::function<void(Particle &, Particle &)> &function);

    const std::vector<Particle> &getParticles() const;

    void setParticles(const std::vector<Particle> &newParticles);

    void remove(Particle &particleToRemove);
};


#endif //PSEMOLDYN_GROUPF_PARTICLECONTAINER_H
