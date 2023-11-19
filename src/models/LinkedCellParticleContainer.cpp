//
// Created by berks on 17.11.2023.
//

#include "LinkedCellParticleContainer.h"
#include "Particle.h"

LinkedCellParticleContainer::LinkedCellParticleContainer(int xSize, int ySize, int zSize, int cellSize) : xSize(xSize), ySize(ySize), zSize(zSize), cellSize(cellSize) {
    this->xSize = xSize;
    this->ySize = ySize;
    this->zSize = zSize;
    this->cellSize = cellSize;

    int numberOfCells = xSize * ySize * zSize;
    cells = std::vector<std::vector<Particle>>(numberOfCells);
}

LinkedCellParticleContainer::~LinkedCellParticleContainer() {

}

void LinkedCellParticleContainer::applyToAllPairsOnce(const std::function<void(Particle&, Particle&)>& function) {
    // Iterate through cells in the container
    for (int cellIndex = 0; cellIndex < cells.size(); cellIndex++) {
        // Calculate x, y, z indices from cellIndex
        int xIndex = cellIndex % xSize;
        int yIndex = (cellIndex / xSize) % ySize;
        int zIndex = cellIndex / (xSize * ySize);

        // Iterate through particles in the current cell
        for (auto& particle : cells[cellIndex]) {

            // Iterate through the eight neighboring cells
            for (int neighborOffset = 0; neighborOffset < 8; neighborOffset++) {
                int neighborX = xIndex + neighborOffset % 2;
                int neighborY = yIndex + (neighborOffset / 2) % 2;
                int neighborZ = zIndex + neighborOffset / 4;

                // Check if the neighboring cell is within bounds
                if (neighborX >= 0 && neighborX < xSize &&
                    neighborY >= 0 && neighborY < ySize &&
                    neighborZ >= 0 && neighborZ < zSize) {

                    int neighborCellIndex = neighborX + neighborY * xSize + neighborZ * xSize * ySize;

                    // Iterate through particles in the neighboring cell
                    for (auto& neighborParticle : cells[neighborCellIndex]) {
                        function(particle, neighborParticle);
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

    // remove particle from cell
    for (int particleIndex = 0; particleIndex < cell.size(); particleIndex++) {
        if (cell[particleIndex] == particle) {
            cell.erase(cell.begin() + particleIndex);
            break;
        }
    }
}

/**
 * @brief Add the particle to the specified cell.
 *
 * @param cellIndex The index of the cell to which the particle should be added.
 * @param particle The particle to be added to the cell.
 */
void LinkedCellParticleContainer::addParticleToCell(int cellIndex, Particle &particle) {
    cells[cellIndex].push_back(particle);
}

void LinkedCellParticleContainer::updateParticleCell(int cellIndex) {

    for (int particleIndex = 0; particleIndex < cells[cellIndex].size(); particleIndex++) {
        auto particle = cells[cellIndex][particleIndex];
        // Calculate new cell index based on the current position
        int xIndex = static_cast<int>(particle.getX()[0] / cellSize);
        int yIndex = static_cast<int>(particle.getX()[1] / cellSize);
        int zIndex = static_cast<int>(particle.getX()[2] / cellSize);

        int newCellIndex = xIndex + yIndex * xSize + zIndex * xSize * ySize;

        if (newCellIndex != cellIndex) {
            // Remove the particle from the current cell
            removeParticleFromCell(cellIndex, particle);

            // Add the particle to the new cell
            addParticleToCell(newCellIndex, particle);
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
