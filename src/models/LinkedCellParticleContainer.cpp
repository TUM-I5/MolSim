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
        // Iterate through particles in the cell
        for (auto& particle : cells[cellIndex]) {
            // Iterate through neighboring cells
            for (int neighborIndex : getNeighboringCellIndices(cellIndex)) {
                // Iterate through particles in the neighboring cell
                for (auto& neighborParticle : cells[neighborIndex]) {
                    // Avoid duplicate pairs and ensure the particle indices are in order
                    std::array<double, 3> diffVector = particle.diffTo(neighborParticle);

                    if (diffVector[0] > 0 || (diffVector[0] == 0 && diffVector[1] > 0) || (diffVector[0] == 0 && diffVector[1] == 0 && diffVector[2] > 0)) {
                        function(particle, neighborParticle);
                    }
                }
            }
        }
    }
}

// Helper function to get neighboring cell indices
std::vector<int> LinkedCellParticleContainer::getNeighboringCellIndices(int cellIndex) const {
    std::vector<int> neighborIndices;

    // Calculate x, y, z indices from cellIndex
    int xIndex = cellIndex % xSize;
    int yIndex = (cellIndex / xSize) % ySize;
    int zIndex = cellIndex / (xSize * ySize);

    // Iterate through neighboring cells
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dz = -1; dz <= 1; dz++) {
                int neighborX = xIndex + dx;
                int neighborY = yIndex + dy;
                int neighborZ = zIndex + dz;

                // Check if the neighboring cell is within bounds
                if (neighborX >= 0 && neighborX < xSize &&
                    neighborY >= 0 && neighborY < ySize &&
                    neighborZ >= 0 && neighborZ < zSize) {
                    int neighborCellIndex = neighborX + neighborY * xSize + neighborZ * xSize * ySize;
                    neighborIndices.push_back(neighborCellIndex);
                }
            }
        }
    }

    return neighborIndices;
}


