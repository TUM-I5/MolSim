//
// Created by berks on 17.11.2023.
//

#include "LinkedCellParticleContainer.h"

#include <iostream>
#include <spdlog/spdlog.h>

#include "Particle.h"
#include "../utils/ArrayUtils.h"

LinkedCellParticleContainer::LinkedCellParticleContainer(int xSize, int ySize, int zSize, int cellSize, double deltaT) : xSize(xSize), ySize(ySize), zSize(zSize), cellSize(cellSize), deltaT(deltaT) {
    xCells = static_cast<int>(std::ceil(xSize / cellSize)) + 2;
    yCells = static_cast<int>(std::ceil(ySize / cellSize)) + 2;
    zCells = static_cast<int>(std::ceil(zSize / cellSize)) + 2;

    //bool isReflectionEnabled = true;

    if (xCells % 2 != 0) {
        xCells += 1;
    }

    if (yCells % 2 != 0) {
        yCells += 1;
    }

    if (zCells % 2 != 0) {
        zCells += 1;
    }

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
            boundaryCellIndices.insert(index3dTo1d(x, y, 1));                 // Bottom boundary
            boundaryCellIndices.insert(index3dTo1d(x, y, zCells - 2));        // Top boundary
        }
    }

    for (int x = 1; x < xCells - 1; x++) {
        for (int z = 1; z < zCells - 1; z++) {
            boundaryCellIndices.insert(index3dTo1d(x, 1, z));                 // Front boundary
            boundaryCellIndices.insert(index3dTo1d(x, yCells - 2, z));        // Back boundary
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
    int xIndex = static_cast<int>((std::floor((particle.getX()[0] + (xSize / 2)) / cellSize)));
    int yIndex = static_cast<int>((std::floor((particle.getX()[1] + (ySize / 2)) / cellSize)));
    int zIndex = static_cast<int>((std::floor((particle.getX()[2] + (zSize / 2)) / cellSize)));

    if (xIndex < 0 || xIndex >= xCells || yIndex < 0 || yIndex >= yCells || zIndex < 0 || zIndex >= zCells) {
        spdlog::info("Particle out of bounds: {}, {}, {}", particle.getX()[0], particle.getX()[1], particle.getX()[2]);
        // -1 means halo cell
        return -1;
    }

    return xIndex + yIndex * xCells + zIndex * xCells * yCells;
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
            updateParticleCell(cellIndex, true);
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

void LinkedCellParticleContainer::updateParticleCell(int cellIndex, bool isReflectionEnabled) {
    auto &cell = cells[cellIndex];

    for (auto it = cell.begin(); it != cell.end();) {
        if(isReflectionEnabled) {
            counterParticleOnReflection(*it);
        }
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

void LinkedCellParticleContainer::reflectIfNecessaryOnAxis(Particle& particle, double particleNextPos, double axisMin, double axisMax, int axisIndex) {
    if(particleNextPos <= axisMin) {
        particle.setX(updatePositionOnReflection(particle.getX(), axisIndex, axisMin));
        particle.setV(updateVelocityOnReflection(particle.getV(), axisIndex));
    } else if(particleNextPos >= axisMax) {
        particle.setX(updatePositionOnReflection(particle.getX(), axisIndex, axisMax));
        particle.setV(updateVelocityOnReflection(particle.getV(), axisIndex));
    } else {
        particle.setX(particle.getX() + (deltaT * particle.getV()));
    }
}

void LinkedCellParticleContainer::counterParticleOnReflection(Particle& particle) {
    double particleNextXPos = particle.getX()[0] + (deltaT * particle.getV()[0]);
    double particleNextYPos = particle.getX()[1] + (deltaT * particle.getV()[1]);
    double particleNextZPos = particle.getX()[2] + (deltaT * particle.getV()[2]);

    double xMax = static_cast<double>(xSize) / 2.0;
    double xMin = -static_cast<double>(xSize) / 2.0;
    double yMax = static_cast<double>(ySize) / 2.0;
    double yMin = -static_cast<double>(ySize) / 2.0;
    double zMax = static_cast<double>(zSize) / 2.0;
    double zMin = -static_cast<double>(zSize) / 2.0;

    double particleNextMovementDistance = ArrayUtils::L2Norm(deltaT * particle.getV());

    while(particleNextXPos <= xMin || particleNextXPos >= xMax || particleNextYPos <= yMin ||
          particleNextYPos >= yMax || particleNextZPos <= zMin || particleNextZPos >= zMax) {

        reflectIfNecessaryOnAxis(particle, particleNextXPos, xMin, xMax, 0);
        reflectIfNecessaryOnAxis(particle, particleNextYPos, yMin, yMax, 1);
        reflectIfNecessaryOnAxis(particle, particleNextZPos, zMin, zMax, 2);

        particleNextXPos = particle.getX()[0];
        particleNextYPos = particle.getX()[1];
        particleNextZPos = particle.getX()[2];
    }

}


//Useless at the moment, will be deleted if no future use can be found
void LinkedCellParticleContainer::handleBoundaries(const std::function<void(Particle&)>& function) {
    for (int cellIndex : boundaryCellIndices) {
        for (auto& particle : cells[cellIndex]) {
            function(particle);
        }
    }
}









