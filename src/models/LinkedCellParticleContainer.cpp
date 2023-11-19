//
// Created by berks on 17.11.2023.
//

#include "LinkedCellParticleContainer.h"

#include <iostream>

#include "Particle.h"

LinkedCellParticleContainer::LinkedCellParticleContainer(int xSize, int ySize, int zSize, int cellSize) : xSize(xSize), ySize(ySize), zSize(zSize), cellSize(cellSize) {
    xCells = xSize / cellSize;
    yCells = ySize / cellSize;
    zCells = zSize / cellSize;

    int numberOfCells = xCells * yCells * zCells;

    std::cout << "Number of cells: " << numberOfCells << std::endl;

    cells = std::vector<std::vector<Particle>>(numberOfCells);
}

LinkedCellParticleContainer::~LinkedCellParticleContainer() {

}

int LinkedCellParticleContainer::index3dTo1d(int x, int y, int z) {
    return x + y * xCells + z * xCells * yCells;
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
        bool neighborsInsideCellIterated = false;

        // Calculate x, y, z indices from cellIndex

        auto coords = index1dTo3d(cellIndex);

        for (auto &p1 : cells[cellIndex]) {
            for (auto &p2 : cells[cellIndex]) {
                if (p1 == p2) {
                    // same particle. skip.
                    continue;
                }

                function(p1, p2);
            }
        }

        for (int x = 0; x <= 1; x++) {
            for (int y = 0; y <= 1; y++) {
                for (int z = 0; z <= 1; z++) {
                    int neighborX = coords[0] + x;
                    int neighborY = coords[1] + y;
                    int neighborZ = coords[2] + z;

                    if (
                        neighborX < 0 || neighborX >= xCells
                        || neighborY < 0 || neighborY >= yCells
                        || neighborZ < 0 || neighborZ >= zCells
                    ) continue;

                    if (x == 0 && y == 0 && z == 0) continue;

                    auto currentCell = cells[index3dTo1d(neighborX, neighborY, neighborZ)];

                    for (auto &p1 : cells[cellIndex]) {
                        for (auto &p2 : currentCell) {
                            function(p1, p2);
                        }
                    }
                }
            }
        }

        /*

        // Iterate through particles in the current cell
        for (int particleIndex = 0; particleIndex < cells[cellIndex].size(); particleIndex++) {

            // Iterate through the eight neighboring cells
            for (int neighborOffset = 0; neighborOffset < 8; neighborOffset++) {
                int neighborX = xIndex + neighborOffset % 2;
                int neighborY = yIndex + (neighborOffset / 2) % 2;
                int neighborZ = zIndex + neighborOffset / 4;

                // Check if the neighboring cell is within bounds
                if (neighborX >= 0 && neighborX < xCells &&
                    neighborY >= 0 && neighborY < yCells &&
                    neighborZ >= 0 && neighborZ < zCells && (neighborOffset != 0 || !neighborsInsideCellIterated)) {

                    int neighborCellIndex = neighborX + neighborY * xCells + neighborZ * xCells * yCells;

                    // Iterate through particles in the neighboring cell
                    for (int neighborParticleIndex = 0; neighborParticleIndex < cells[neighborCellIndex].size(); neighborParticleIndex++) {
                        function(cells[cellIndex][particleIndex], cells[neighborCellIndex][neighborParticleIndex]);
                    }

                    if(neighborOffset == 0) {
                        neighborsInsideCellIterated = true;
                    }
                }
            }
        }
        */
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

    // remove particle from cell
    for (int particleIndex = 0; particleIndex < cell.size(); particleIndex++) {
        if (cell[particleIndex] == particle) {
            cell.erase(cell.begin() + particleIndex);
            break;
        }
    }
}

int LinkedCellParticleContainer::cellIndexForParticle(const Particle &particle) {
    int xIndex = static_cast<int>(particle.getX()[0] / cellSize);
    int yIndex = static_cast<int>(particle.getX()[1] / cellSize);
    int zIndex = static_cast<int>(particle.getX()[2] / cellSize);

   return xIndex + yIndex * xCells + zIndex * xCells * yCells;
}

void LinkedCellParticleContainer::add(const Particle &particle) {
    addParticleToCell(cellIndexForParticle(particle), particle);
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
    std::vector<Particle> movedParticles;
    auto &cell = cells[cellIndex];

    for (int particleIndex = 0; particleIndex < cell.size(); particleIndex++) {
        auto particle = cell[particleIndex];

        int newCellIndex = cellIndexForParticle(particle);

        if (newCellIndex != cellIndex) {
            // Add the particle to movedParticles
            movedParticles.push_back(particle);
        }
    }

    // Remove moved particles from the current cell and add them to their new cells
    for (auto particle : movedParticles) { // Note: not a reference
        addParticleToCell(cellIndexForParticle(particle), particle);
        removeParticleFromCell(cellIndex, particle);
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
