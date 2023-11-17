//
// Created by berks on 17.11.2023.
//

#pragma once

#include <vector>
#include <list>
#include <array>
#include "Particle.h"
#include "ParticleContainer.h"

class LinkedCellParticleContainer : public ParticleContainer{
private:

    int xSize;
    int ySize;
    int zSize;

    int cellSize;
    /**
     * The vector that contains all the particles in the container
     */

    std::vector<std::list<Particle>> cells;

public:

    LinkedCellParticleContainer(int xSize, int ySize, int zSize, int cellSize);

    ~LinkedCellParticleContainer();

    void updateCellLists();

    void applyToAllPairsOnce(const std::function<void(Particle &, Particle &)> &function);

    std::vector<int> getNeighboringCellIndices(int cellIndex) const;
};


