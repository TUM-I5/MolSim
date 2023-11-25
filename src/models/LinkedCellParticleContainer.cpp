//
// Created by berks on 17.11.2023.
//

#include "LinkedCellParticleContainer.h"

#include <iostream>
#include <spdlog/spdlog.h>

#include "Particle.h"

LinkedCellParticleContainer::LinkedCellParticleContainer(int xSize, int ySize, int zSize, int cellSize) : xSize(xSize), ySize(ySize), zSize(zSize), cellSize(cellSize) {
    xCells = xSize / cellSize;
    yCells = ySize / cellSize;
    zCells = zSize / cellSize;

    int numberOfCells = xCells * yCells * zCells;

    cells = std::vector<std::vector<Particle>>(numberOfCells);

    haloCell = std::vector<Particle>();
}

LinkedCellParticleContainer::~LinkedCellParticleContainer() {

}

int LinkedCellParticleContainer::index3dTo1d(int x, int y, int z) {
    return (x + y * xCells + z * xCells * yCells);
}

std::array<int, 3> LinkedCellParticleContainer::index1dTo3d(int index) {
    int x = index % xCells;
    int y = (index / xCells) % yCells;
    int z = index / (xCells * yCells);

    return {x, y, z};
}

void LinkedCellParticleContainer::applyToAllPairsOnce(const std::function<void(Particle&, Particle&)>& function) {
    // Iterate through cells in the container
    for (int cellIndex = 0; cellIndex < cells.size(); cellIndex++) {
        // Calculate x, y, z indices from cellIndex

        auto coords = index1dTo3d(cellIndex);

        auto &firstCell = cells[cellIndex];

        for (int i = 0; i < firstCell.size(); i++) {
            for (int j = i + 1; j < firstCell.size(); j++) {
                function(firstCell[i], firstCell[j]);
            }
        }


        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                for (int z = -1; z <= 1; z++) {
                    int neighborX = coords[0] + x;
                    int neighborY = coords[1] + y;
                    int neighborZ = coords[2] + z;

                    if (
                        neighborX < 0 || neighborX >= xCells
                        || neighborY < 0 || neighborY >= yCells
                        || neighborZ < 0 || neighborZ >= zCells
                    ) continue;

                    if (x == 0 && y == 0 && z == 0) continue;

                    auto &currentCell = cells[index3dTo1d(neighborX, neighborY, neighborZ)];

                    for (auto &p1 : firstCell) {
                        for (auto &p2 : currentCell) {
                            function(p1, p2);
                        }
                    }
                }
            }
        }
    }
}

void LinkedCellParticleContainer::applyToAll(const std::function<void(Particle &)> &function) {
   // iterate over cells
   for (auto &cell : cells) {
       // iterate over particles in cell
       for (auto &particle : cell) {
           function(particle);
       }
   }
}

void LinkedCellParticleContainer::applyToAll(const std::function<void(Particle &)> &function, bool updateCells) {
    // iterate over cells
    for (int cellIndex = 0; cellIndex < cells.size(); cellIndex++) {
        // iterate over particles in cell
        for (auto &particle : cells[cellIndex]) {
            function(particle);
        }

        if(updateCells) {
            updateParticleCell(cellIndex);
        }
    }
}


/**
     * @brief Remove the particle from the specified cell.
     *
     * @param cellIndex The index of the cell from which to remove the particle.
     * @param particle The particle to be removed from the cell.
     */
void LinkedCellParticleContainer::removeParticleFromCell(int cellIndex, Particle &particle) {
    auto &cell = cells[cellIndex];

    for (auto it = cell.begin(); it != cell.end(); ++it) {
        if (*it == particle) {
            cell.erase(it);
            break;
        }
    }
}


int LinkedCellParticleContainer::cellIndexForParticle(const Particle &particle) {
    int xIndex = static_cast<int>((std::floor((particle.getX()[0] + (xSize / 2)) / cellSize)));
    int yIndex = static_cast<int>((std::floor((particle.getX()[1] + (ySize / 2)) / cellSize)));
    int zIndex = static_cast<int>((std::floor((particle.getX()[2] + (zSize / 2)) / cellSize)));

    if (xIndex < 0 || xIndex >= xCells || yIndex < 0 || yIndex >= yCells || zIndex < 0 || zIndex >= zCells) {
        spdlog::info("Particle out of bounds: {}, {}, {}, moving to the halo cell", particle.getX()[0], particle.getX()[1], particle.getX()[2]);
        // -1 means halo cell
        return -1;
    }


    return xIndex + yIndex * xCells + zIndex * xCells * yCells;
}

void LinkedCellParticleContainer::add(const Particle &particle) {
    int cellIndex = cellIndexForParticle(particle);

    // out of bounds: should go into the halo cell
    if (cellIndex == -1) {
        haloCell.push_back(particle);
        return;
    }

    addParticleToCell(cellIndex, particle);
}

/**
 * @brief Add the particle to the specified cell.
 *
 * @param cellIndex The index of the cell to which the particle should be added.
 * @param particle The particle to be added to the cell.
 */
void LinkedCellParticleContainer::addParticleToCell(int cellIndex, const Particle &particle) {
    cells[cellIndex].push_back(particle);
}

void LinkedCellParticleContainer::updateParticleCell(int cellIndex) {
    auto &cell = cells[cellIndex];
    for (auto it = cell.begin(); it != cell.end();) {
        int newCellIndex = cellIndexForParticle(*it);

        if (newCellIndex != cellIndex) {
            add(*it);
            it = cell.erase(it); // Remove the particle from the old cell
        } else {
            ++it;
        }
    }
}


void LinkedCellParticleContainer::applyBoundaryConditions(Particle &particle, double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) {
    // Check X-axis boundary
    if (particle.getX()[0] <= xMin || particle.getX()[0] >= xMax) {
        particle.setV({-particle.getV()[0], particle.getV()[1], particle.getV()[2]});
    }

    // Check Y-axis boundary
    if (particle.getX()[1] <= yMin || particle.getX()[1] >= yMax) {
        particle.setV({particle.getV()[0], -particle.getV()[1], particle.getV()[2]});
    }

    // Check Z-axis boundary
    if (particle.getX()[2] <= zMin || particle.getX()[2] >= zMax) {
        particle.setV({particle.getV()[0], particle.getV()[1], -particle.getV()[2]});
    }
}

int LinkedCellParticleContainer::size() {
    int result = 0;

    for(auto &cell : cells) {
        result += static_cast<int>(cell.size());
    }

    return result;
}