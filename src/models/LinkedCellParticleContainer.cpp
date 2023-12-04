//
// Created by berks on 17.11.2023.
//

#include "LinkedCellParticleContainer.h"

#include <iostream>
#include <spdlog/spdlog.h>

#include "Particle.h"
#include "../utils/ArrayUtils.h"

LinkedCellParticleContainer::LinkedCellParticleContainer(double xSize, double ySize, double zSize, double cutoffRadius, double deltaT,
                                                         BoundaryBehavior boundaryBehaviorTop,
                                                         BoundaryBehavior boundaryBehaviorBottom,
                                                         BoundaryBehavior boundaryBehaviorRight,
                                                         BoundaryBehavior boundaryBehaviorLeft,
                                                         BoundaryBehavior boundaryBehaviorFront,
                                                         BoundaryBehavior boundaryBehaviorBack)
        :
        xSize(xSize), ySize(ySize), zSize(zSize),
        xCells(static_cast<int>(std::floor(xSize / cutoffRadius))),
        yCells(static_cast<int>(std::floor(ySize / cutoffRadius))),
        zCells(static_cast<int>(std::floor(zSize / cutoffRadius))),
        cutoffRadius(cutoffRadius),
        deltaT(deltaT), boundaryBehaviorTop(boundaryBehaviorTop),
          boundaryBehaviorBottom(boundaryBehaviorBottom), boundaryBehaviorRight(boundaryBehaviorRight),
          boundaryBehaviorLeft(boundaryBehaviorLeft), boundaryBehaviorFront(boundaryBehaviorFront),
          boundaryBehaviorBack(boundaryBehaviorBack) {

    cellXSize = xSize / xCells;
    cellYSize = ySize / yCells;
    cellZSize = zSize / zCells;

    xCells += 2;
    yCells += 2;
    zCells += 2;

    int numberOfCells = xCells * yCells * zCells;

    cells = std::vector<std::vector<Particle>>(numberOfCells);

    isHaloCellVector = std::vector<bool>(numberOfCells, true);

    boundaryCellIndices = std::set<int>();
    haloCellIndices = std::set<int>();

    // Precompute cells-vector indices of the halo cells in and store them in haloCellIndices
    for (int x = 0; x < xCells; x++) {
        for (int y = 0; y < yCells; y++) {
            haloCellIndices.insert(index3dTo1d(x, y, 0));                 // Bottom halo
            haloCellIndices.insert(index3dTo1d(x, y, zCells - 1));        // Top halo
        }
    }

    for (int x = 0; x < xCells; x++) {
        for (int z = 0; z < zCells; z++) {
            haloCellIndices.insert(index3dTo1d(x, 0, z));                 // Front halo
            haloCellIndices.insert(index3dTo1d(x, yCells - 1, z));        // Back halo
        }
    }

    for (int y = 0; y < yCells; y++) {
        for (int z = 0; z < zCells; z++) {
            haloCellIndices.insert(index3dTo1d(0, y, z));                 // Left halo
            haloCellIndices.insert(index3dTo1d(xCells - 1, y, z));        // Right halo
        }
    }


    // Precompute cells-vector indices of the boundary cells in and store them in boundaryCellIndices
    for (int x = 1; x < xCells - 1; x++) {
        for (int y = 1; y < yCells - 1; y++) {
            boundaryCellIndices.insert(index3dTo1d(x, y, 1));                 // Front boundary
            boundaryCellIndices.insert(index3dTo1d(x, y, zCells - 2));        // Back boundary
        }
    }

    for (int x = 1; x < xCells - 1; x++) {
        for (int z = 1; z < zCells - 1; z++) {
            boundaryCellIndices.insert(index3dTo1d(x, 1, z));                 // Bottom boundary
            boundaryCellIndices.insert(index3dTo1d(x, yCells - 2, z));        // Top boundary
        }
    }

    for (int y = 1; y < yCells - 1; y++) {
        for (int z = 1; z < zCells - 1; z++) {
            boundaryCellIndices.insert(index3dTo1d(1, y, z));                 // Left boundary
            boundaryCellIndices.insert(index3dTo1d(xCells - 2, y, z));        // Right boundary
        }
    }

    // Iterate through haloCellIndices and set corresponding pairs to false
    for (int haloIndex : haloCellIndices) {
        isHaloCellVector[haloIndex] = false;
    }

}

LinkedCellParticleContainer::LinkedCellParticleContainer(double xSize, double ySize, double zSize, double cutoffRadius, double deltaT)
: LinkedCellParticleContainer(xSize, ySize, zSize, cutoffRadius, deltaT, BoundaryBehavior::Reflective) {}

LinkedCellParticleContainer::LinkedCellParticleContainer(double xSize, double ySize, double zSize, double cutoffRadius, double deltaT, BoundaryBehavior boundaryBehavior)
: LinkedCellParticleContainer(xSize, ySize, zSize, cutoffRadius, deltaT, boundaryBehavior, boundaryBehavior, boundaryBehavior, boundaryBehavior, boundaryBehavior, boundaryBehavior) {}

LinkedCellParticleContainer::~LinkedCellParticleContainer() = default;

int LinkedCellParticleContainer::index3dTo1d(int x, int y, int z) {
    return (x + y * xCells + z * xCells * yCells);
}

std::array<int, 3> LinkedCellParticleContainer::index1dTo3d(int index) {
    int x = index % xCells;
    int y = (index / xCells) % yCells;
    int z = index / (xCells * yCells);

    return {x, y, z};
}

int LinkedCellParticleContainer::cellIndexForParticle(const Particle &particle) {
    int xIndex = static_cast<int>((std::floor((particle.getX()[0]) / cellXSize)));
    int yIndex = static_cast<int>((std::floor((particle.getX()[1]) / cellYSize)));
    int zIndex = static_cast<int>((std::floor((particle.getX()[2]) / cellZSize)));

    if (xIndex < 0 || xIndex >= xCells || yIndex < 0 || yIndex >= yCells || zIndex < 0 || zIndex >= zCells) {
        spdlog::info("Particle out of bounds: {}, {}, {}", particle.getX()[0], particle.getX()[1], particle.getX()[2]);
        // -1 means halo cell
        return -1;
    }

    return (xIndex+1) + (yIndex+1) * xCells + (zIndex+1) * xCells * yCells;
}


void LinkedCellParticleContainer::applyToAllPairsOnce(const std::function<void(Particle&, Particle&)>& function) {
    // Iterate through all cells in the container
    for (int cellIndex = 0; cellIndex < cells.size(); cellIndex++) {
        // Skip halo cells
        if (!isHaloCellVector[cellIndex]) continue;

        auto coords = index1dTo3d(cellIndex);
        auto &firstCell = cells[cellIndex];  // Extract the vector of particles from the pair

        // Iterate through all pairs of particles in the same cell
        for (int i = 0; i < firstCell.size(); i++) {
            for (int j = i + 1; j < firstCell.size(); j++) {
                // Check if the pair has been processed before by comparing memory addresses
                if (&firstCell[i] < &firstCell[j]) {
                    function(firstCell[i], firstCell[j]);
                }
            }
        }

        // Iterate through neighboring cells
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                for (int z = -1; z <= 1; z++) {
                    int neighborX = coords[0] + x;
                    int neighborY = coords[1] + y;
                    int neighborZ = coords[2] + z;

                    if (neighborX <= 0 || neighborX >= xCells - 1
                        || neighborY <= 0 || neighborY >= yCells - 1
                        || neighborZ <= 0 || neighborZ >= zCells - 1) continue;

                    if (x == 0 && y == 0 && z == 0) continue;

                    auto &currentCell = cells[index3dTo1d(neighborX, neighborY, neighborZ)];  // Extract vector from the pair

                    for (auto &p1 : firstCell) {
                        for (auto &p2 : currentCell) {
                            // Check if the pair has been processed before by comparing memory addresses
                            if (&p1 < &p2) {
                                function(p1, p2);
                            }
                        }
                    }
                }
            }
        }
    }
}

void LinkedCellParticleContainer::applyToAll(const std::function<void(Particle&)>& function) {
    for (int cellIndex = 0; cellIndex < cells.size(); cellIndex++) {
        if (!isHaloCellVector[cellIndex]) continue;  // Skip processing for halo cells

        for (auto& particle : cells[cellIndex]) {
            function(particle);
        }
    }
}

void LinkedCellParticleContainer::applyToAll(const std::function<void(Particle&)>& function, bool updateCells) {
    for (int cellIndex = 0; cellIndex < cells.size(); cellIndex++) {
        if (!isHaloCellVector[cellIndex]) continue;  // Skip processing for halo cells

        for (auto& particle : cells[cellIndex]) {
            function(particle);
        }

        if (updateCells) {
            updateParticleCell(cellIndex);
        }
    }
}

void LinkedCellParticleContainer::add(const Particle &particle) {
    int cellIndex = cellIndexForParticle(particle);

    if (cellIndex != -1) {
        addParticleToCell(cellIndex, particle);
    } else {
        spdlog::info("Particle out of bounds: {}, {}, {}", particle.getX()[0], particle.getX()[1], particle.getX()[2]);
    }

}

void LinkedCellParticleContainer::addParticleToCell(int cellIndex, const Particle &particle) {
    cells[cellIndex].push_back(particle);
}

void LinkedCellParticleContainer::updateParticleCell(int cellIndex) {
    auto &cell = cells[cellIndex];

    for (auto it = cell.begin(); it != cell.end();) {

        vectorReverseReflection(*it);

        int newCellIndex = cellIndexForParticle(*it);

        if (newCellIndex != cellIndex) {
            add(*it);
            it = cell.erase(it);  // Remove the particle from the old cell
        } else {
            ++it;
        }
    }
}

void LinkedCellParticleContainer::deleteParticlesInHaloCells() {
    // Iterate through halo cells
    for (int haloIndex : haloCellIndices) {
        int cellIndex = haloIndex;

        if (isHaloCellVector[haloIndex]) {
            // Delete particles in the halo cell
            cells[cellIndex].clear();
        }
    }
}

int LinkedCellParticleContainer::size() {
    int size = 0;

    for (int cellIndex = 0; cellIndex < cells.size(); cellIndex++) {
        if (!isHaloCellVector[cellIndex]) continue;  // Skip processing for halo cells

        size += static_cast<int>(cells[cellIndex].size());
    }

    return size;
}

std::array<double, 3> LinkedCellParticleContainer::updateVelocityOnReflection(const std::array<double, 3> velocity, int axisIndex) {
    std::array<double, 3> updatedVelocity = {velocity[0], velocity[1], velocity[2]};
    updatedVelocity[axisIndex] = -velocity[axisIndex];
    return updatedVelocity;
}

std::array<double, 3> LinkedCellParticleContainer::updatePositionOnReflection(const std::array<double, 3>& position, int axisIndex, double boundary) {
    std::array<double, 3> updatedPosition = {position[0], position[1], position[2]};
    updatedPosition[axisIndex] = 2 * boundary - position[axisIndex];
    return updatedPosition;
}

bool LinkedCellParticleContainer::reflectIfNecessaryOnAxis(Particle& particle, double axisMin, double axisMax, int axisIndex) {
    if (axisIndex == 0 && boundaryBehaviorRight == BoundaryBehavior::Reflective && particle.getX()[axisIndex] >= axisMax) {
        particle.setX(updatePositionOnReflection(particle.getX(), axisIndex, axisMax));
        particle.setV(updateVelocityOnReflection(particle.getV(), axisIndex));
        return true;
    } else if (axisIndex == 0 && boundaryBehaviorLeft == BoundaryBehavior::Reflective && particle.getX()[axisIndex] <= axisMin) {
        particle.setX(updatePositionOnReflection(particle.getX(), axisIndex, axisMin));
        particle.setV(updateVelocityOnReflection(particle.getV(), axisIndex));
        return true;
    } else if (axisIndex == 1 && boundaryBehaviorTop == BoundaryBehavior::Reflective && particle.getX()[axisIndex] >= axisMax) {
        particle.setX(updatePositionOnReflection(particle.getX(), axisIndex, axisMax));
        particle.setV(updateVelocityOnReflection(particle.getV(), axisIndex));
        return true;
    } else if (axisIndex == 1 && boundaryBehaviorBottom == BoundaryBehavior::Reflective && particle.getX()[axisIndex] <= axisMin) {
        particle.setX(updatePositionOnReflection(particle.getX(), axisIndex, axisMin));
        particle.setV(updateVelocityOnReflection(particle.getV(), axisIndex));
        return true;
    } else if (axisIndex == 2 && boundaryBehaviorFront == BoundaryBehavior::Reflective && particle.getX()[axisIndex] >= axisMax) {
        particle.setX(updatePositionOnReflection(particle.getX(), axisIndex, axisMax));
        particle.setV(updateVelocityOnReflection(particle.getV(), axisIndex));
        return true;
    } else if (axisIndex == 2 && boundaryBehaviorBack == BoundaryBehavior::Reflective && particle.getX()[axisIndex] <= axisMin) {
        particle.setX(updatePositionOnReflection(particle.getX(), axisIndex, axisMin));
        particle.setV(updateVelocityOnReflection(particle.getV(), axisIndex));
        return true;
    } else {
        return false;
    }
}

void LinkedCellParticleContainer::vectorReverseReflection(Particle& particle) {
    bool reflected = false;

    while(true) {
        reflected = false;

        reflected = reflectIfNecessaryOnAxis(particle,  0, xSize, 0);

        reflected = reflected || reflectIfNecessaryOnAxis(particle, 0, ySize, 1);

        reflected = reflected || reflectIfNecessaryOnAxis(particle,  0, zSize, 2);

        if (!reflected) {
            break;
        }
    }
}

double LinkedCellParticleContainer::getXSize() const {
    return xSize;
}

double LinkedCellParticleContainer::getYSize() const {
    return ySize;
}

double LinkedCellParticleContainer::getZSize() const {
    return zSize;
}

int LinkedCellParticleContainer::getXCells() const {
    return xCells;
}

int LinkedCellParticleContainer::getYCells() const {
    return yCells;
}

int LinkedCellParticleContainer::getZCells() const {
    return zCells;
}

double LinkedCellParticleContainer::getCutoffRadius() const {
    return cutoffRadius;
}

double LinkedCellParticleContainer::getCellXSize() const {
    return cellXSize;
}

double LinkedCellParticleContainer::getCellYSize() const {
    return cellYSize;
}

double LinkedCellParticleContainer::getCellZSize() const {
    return cellZSize;
}

double LinkedCellParticleContainer::getDeltaT() const {
    return deltaT;
}

const std::vector<std::vector<Particle>> &LinkedCellParticleContainer::getCells() const {
    return cells;
}

const std::set<int> &LinkedCellParticleContainer::getBoundaryCellIndices() const {
    return boundaryCellIndices;
}

const std::set<int> &LinkedCellParticleContainer::getHaloCellIndices() const {
    return haloCellIndices;
}

const std::vector<bool> &LinkedCellParticleContainer::getIsHaloCellVector() const {
    return isHaloCellVector;
}

BoundaryBehavior LinkedCellParticleContainer::getBoundaryBehaviorTop() const {
    return boundaryBehaviorTop;
}

BoundaryBehavior LinkedCellParticleContainer::getBoundaryBehaviorBottom() const {
    return boundaryBehaviorBottom;
}

BoundaryBehavior LinkedCellParticleContainer::getBoundaryBehaviorRight() const {
    return boundaryBehaviorRight;
}

BoundaryBehavior LinkedCellParticleContainer::getBoundaryBehaviorLeft() const {
    return boundaryBehaviorLeft;
}

BoundaryBehavior LinkedCellParticleContainer::getBoundaryBehaviorFront() const {
    return boundaryBehaviorFront;
}

BoundaryBehavior LinkedCellParticleContainer::getBoundaryBehaviorBack() const {
    return boundaryBehaviorBack;
}
