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
    cells = std::vector<std::list<Particle>>(numberOfCells);
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




