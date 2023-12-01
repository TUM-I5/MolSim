//
// Created by berks on 17.11.2023.
//

#pragma once

#include <vector>
#include <array>
#include <list>
#include <set>
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

    std::set<int> boundaryCellIndices;

    std::set<int> haloCellIndices;

    std::vector<bool> isHaloCellVector;


public:

    LinkedCellParticleContainer(int xSize, int ySize, int zSize, int cellSize);

    ~LinkedCellParticleContainer();

    int index3dTo1d(int x, int y, int z);

    std::array<int, 3> index1dTo3d(int index);

    virtual void applyToAllPairsOnce(const std::function<void(Particle &, Particle &)> &function);

    virtual void applyToAll(const std::function<void(Particle &)> &function);

    int cellIndexForParticle(const Particle &particle);

    void add(const Particle &particle);

    void updateParticleCell(int cellIndex, bool isReflectionEnabled);

    void applyToAll(const std::function<void(Particle &)> &function, bool updateCells);

    void addParticleToCell(int cellIndex, const Particle &particle);

    int size();

    static void
    reflectOnBoundary(Particle &particle, double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);

    static void reflectOnAxisBoundary(Particle &particle, double axisMin, double axisMax, int axisIndex);

    static std::array<double, 3>
    updatePositionOnReflection(const std::array<double, 3> &position, int axisIndex, double boundary);

    static std::array<double, 3> updateVelocityOnReflection(std::array<double, 3> velocity, int axisIndex);

    void deleteParticlesInHaloCells();

    void handleBoundaries(const std::function<void(Particle&)>& function);

    void counterParticleOnReflection(Particle &particle);
};


