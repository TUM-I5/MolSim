//
// Created by berks on 17.11.2023.
//

#pragma once

#include <vector>
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

    std::vector<std::vector<Particle>> cells;

public:

    LinkedCellParticleContainer(int xSize, int ySize, int zSize, int cellSize);

    ~LinkedCellParticleContainer();

    virtual void applyToAllPairsOnce(const std::function<void(Particle &, Particle &)> &function);

    virtual void applyToAll(const std::function<void(Particle &)> &function);

    void applyBoundaryConditions(Particle &particle, double xMin, double xMax, double yMin, double yMax, double zMin,
                                 double zMax);

    void removeParticleFromCell(int cellIndex, const Particle &particle);

    void addParticleToCell(int cellIndex, Particle &particle);

    void updateParticleCell(int cellIndex);

    void removeParticleFromCell(int cellIndex, Particle &particle);

    void applyToAll(const std::function<void(Particle &)> &function, bool updateCells);
};


