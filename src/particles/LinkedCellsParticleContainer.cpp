#include "LinkedCellsParticleContainer.h"

#include <cmath>

#include "io/logger/Logger.h"
#include "particles/Cell.h"

/*
    Methods of the BoundaryIterator
*/

LinkedCellsParticleContainer::BoundaryIterator::BoundaryIterator(std::vector<Cell*>& cells, int cell_index, int particle_index)
    : cells(cells), cell_index(cell_index), particle_index(particle_index) {}

LinkedCellsParticleContainer::BoundaryIterator& LinkedCellsParticleContainer::BoundaryIterator::operator++() {
    if (cell_index == -1 && particle_index == -1) return *this;

    ++particle_index;
    if (static_cast<size_t>(particle_index) >= cells[cell_index]->getParticleReferences().size()) {
        ++cell_index;
        particle_index = 0;
        while (cell_index < static_cast<int>(cells.size()) && cells[cell_index]->getParticleReferences().size() == 0) {
            ++cell_index;
        }
    }
    if (cell_index >= static_cast<int>(cells.size())) {
        cell_index = -1;
        particle_index = -1;
    }

    return *this;
}

Particle& LinkedCellsParticleContainer::BoundaryIterator::operator*() const {
    if (cell_index == -1 || particle_index == -1) std::cout << "ERROR" << std::endl;
    return *(cells[cell_index]->getParticleReferences()[particle_index]);
}

Particle* LinkedCellsParticleContainer::BoundaryIterator::operator->() const {
    if (cell_index == -1 || particle_index == -1) return nullptr;
    return cells[cell_index]->getParticleReferences()[particle_index];
}

bool LinkedCellsParticleContainer::BoundaryIterator::operator==(const BoundaryIterator& other) const {
    return cell_index == other.cell_index && particle_index == other.particle_index;
}

bool LinkedCellsParticleContainer::BoundaryIterator::operator!=(const BoundaryIterator& other) const {
    return cell_index != other.cell_index || particle_index != other.particle_index;
}

/*
    Methods of the LinkedCellsParticleContainer regarding the BoundaryIterator
*/

LinkedCellsParticleContainer::BoundaryIterator LinkedCellsParticleContainer::boundaryBegin() {
    BoundaryIterator tmp = BoundaryIterator(boundary_cell_references, 0, -1);
    ++tmp;
    return tmp;
}

LinkedCellsParticleContainer::BoundaryIterator LinkedCellsParticleContainer::boundaryEnd() {
    return BoundaryIterator(boundary_cell_references, -1, -1);
}

/*
    Methods of the LinkedCellsParticleContainer
*/

LinkedCellsParticleContainer::LinkedCellsParticleContainer(const std::array<double, 3>& size, double cutoff_radius, int n)
    : domain_size(size), cutoff_radius(cutoff_radius) {
    domain_num_cells = {static_cast<int>(std::ceil(size[0] / cutoff_radius)), static_cast<int>(std::ceil(size[1] / cutoff_radius)),
                        static_cast<int>(std::ceil(size[2] / cutoff_radius))};
    cell_size = {size[0] / domain_num_cells[0], size[1] / domain_num_cells[1], size[2] / domain_num_cells[2]};

    particles.reserve(n);
    cells.reserve((domain_num_cells[0] + 2) * (domain_num_cells[1] + 2) * (domain_num_cells[2] + 2));

    for (int cx = -1; cx < domain_num_cells[0] + 1; ++cx) {
        for (int cy = -1; cy < domain_num_cells[1] + 1; ++cy) {
            for (int cz = -1; cz < domain_num_cells[2] + 1; ++cz) {
                if (cx < 0 || cx >= domain_num_cells[0] || cy < 0 || cy >= domain_num_cells[1] || cz < 0 || cz >= domain_num_cells[2]) {
                    Cell newCell(Cell::CellType::HALO);
                    cells.push_back(newCell);
                    halo_cell_references.push_back(&cells.back());
                    continue;
                } else if (cx == 0 || cx == domain_num_cells[0] - 1 || cy == 0 || cy == domain_num_cells[1] - 1 || cz == 0 ||
                           cz == domain_num_cells[2] - 1) {
                    Cell newCell(Cell::CellType::BOUNDARY);
                    cells.push_back(newCell);
                    boundary_cell_references.push_back(&cells.back());
                    continue;
                } else {
                    Cell newCell(Cell::CellType::INNER);
                    cells.push_back(newCell);
                    inner_cell_references.push_back(&cells.back());
                }
            }
        }
    }
}

void LinkedCellsParticleContainer::addParticle(const Particle& p) {
    Cell* cell = particlePosToCell(p.getX());

    if (cell == nullptr) {
        Logger::logger->error("Particle to insert is out of bounds");
        return;
    }

    particles.push_back(p);
    cell->addParticleReference(&particles.back());
}

void LinkedCellsParticleContainer::addParticle(Particle&& p) {
    Cell* cell = particlePosToCell(p.getX());

    if (cell == nullptr) {
        Logger::logger->error("Particle to insert is out of bounds");
        return;
    }

    particles.push_back(std::move(p));
    cell->addParticleReference(&particles.back());
}

void LinkedCellsParticleContainer::applyPairwiseForces(const std::vector<std::unique_ptr<ForceSource>>& force_sources) {
    std::cout << "applyPairwiseForces(const std::vector<std::unique_ptr<ForceSource>>& force_sources) not implemented" << std::endl;
}

int LinkedCellsParticleContainer::cellCoordToCellIndex(int cx, int cy, int cz) const {
    if (cx < -1 || cx > domain_num_cells[0] || cy < -1 || cy > domain_num_cells[1] || cz < -1 || cz > domain_num_cells[2]) {
        return -1;
    }
    return (cx + 1) * (domain_num_cells[1] + 2) * (domain_num_cells[2] + 2) + (cy + 1) * (domain_num_cells[2] + 2) + (cz + 1);
}

Cell* LinkedCellsParticleContainer::particlePosToCell(const std::array<double, 3>& pos) {
    return particlePosToCell(pos[0], pos[1], pos[2]);
}

Cell* LinkedCellsParticleContainer::particlePosToCell(double x, double y, double z) {
    int cx = static_cast<int>(std::floor(x / cell_size[0]));
    int cy = static_cast<int>(std::floor(y / cell_size[1]));
    int cz = static_cast<int>(std::floor(z / cell_size[2]));

    int cell_index = cellCoordToCellIndex(cx, cy, cz);
    if (cell_index == -1) {
        return nullptr;
    }

    return &cells[cell_index];
}