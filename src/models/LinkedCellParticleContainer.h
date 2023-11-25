//
// Created by berks on 17.11.2023.
//

#pragma once

#include <vector>
#include <array>
#include <list>
#include "Particle.h"
#include "ParticleContainer.h"

class LinkedCellParticleContainer : public ParticleContainer{
private:
    int xSize;
    int ySize;
    int zSize;

    int xCells;
    int yCells;
    int zCells;

    int cellSize;
    /**
     * The vector that contains all the particles in the container
     */

    std::vector<std::vector<Particle>> cells;

    std::vector<Particle> haloCell;

public:

    LinkedCellParticleContainer(int xSize, int ySize, int zSize, int cellSize);

    ~LinkedCellParticleContainer();

    int index3dTo1d(int x, int y, int z);

    std::array<int, 3> index1dTo3d(int index);

    virtual void applyToAllPairsOnce(const std::function<void(Particle &, Particle &)> &function);

    virtual void applyToAll(const std::function<void(Particle &)> &function);

    void applyBoundaryConditions(Particle &particle, double xMin, double xMax, double yMin, double yMax, double zMin,
                                 double zMax);


    int cellIndexForParticle(const Particle &particle);

    void removeParticleFromCell(int cellIndex, const Particle &particle);

    void add(const Particle &particle);

    void updateParticleCell(int cellIndex);

    void removeParticleFromCell(int cellIndex, Particle &particle);

    void applyToAll(const std::function<void(Particle &)> &function, bool updateCells);

    void addParticleToCell(int cellIndex, const Particle &particle);

    int size();
};


