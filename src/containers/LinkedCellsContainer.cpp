#include "LinkedCellsContainer.h"

#include <cmath>

#include "cells/Cell.h"
#include "io/logger/Logger.h"

/*
    Methods of the BoundaryIterator
*/

LinkedCellsContainer::BoundaryIterator::BoundaryIterator(std::vector<Cell*>& cells, int cell_index, int particle_index)
    : cells(cells), cell_index(cell_index), particle_index(particle_index) {}

LinkedCellsContainer::BoundaryIterator& LinkedCellsContainer::BoundaryIterator::operator++() {
    // if the iterator is the end iterator, keep it that way
    if (cell_index == -1 && particle_index == -1) return *this;

    ++particle_index;

    // search for the next valid particle index in the cells
    while (cell_index < static_cast<int>(cells.size()) &&
           particle_index >= static_cast<int>(cells[cell_index]->getParticleReferences().size())) {
        ++cell_index;
        particle_index = 0;
    }

    // if the iterator is invalid, set it to the end
    if (cell_index >= static_cast<int>(cells.size())) {
        cell_index = -1;
        particle_index = -1;
    }

    return *this;
}

Particle& LinkedCellsContainer::BoundaryIterator::operator*() const {
    if (cell_index == -1 || particle_index == -1) std::cout << "ERROR" << std::endl;
    return *(cells[cell_index]->getParticleReferences()[particle_index]);
}

Particle* LinkedCellsContainer::BoundaryIterator::operator->() const {
    if (cell_index == -1 || particle_index == -1) return nullptr;
    return cells[cell_index]->getParticleReferences()[particle_index];
}

bool LinkedCellsContainer::BoundaryIterator::operator==(const BoundaryIterator& other) const {
    return cell_index == other.cell_index && particle_index == other.particle_index;
}

bool LinkedCellsContainer::BoundaryIterator::operator!=(const BoundaryIterator& other) const {
    return cell_index != other.cell_index || particle_index != other.particle_index;
}

/*
    Methods of the LinkedCellsContainer regarding the BoundaryIterator
*/

LinkedCellsContainer::BoundaryIterator LinkedCellsContainer::boundaryBegin() {
    // initialize the iterator to an invalid state, so that the first call to ++ returns the first valid particle
    BoundaryIterator tmp = BoundaryIterator(boundary_cell_references, 0, -1);
    ++tmp;
    return tmp;
}

LinkedCellsContainer::BoundaryIterator LinkedCellsContainer::boundaryEnd() {
    // create an iterator to the end of the boundary particles
    return BoundaryIterator(boundary_cell_references, -1, -1);
}

/*
    Methods of the LinkedCellsContainer
*/

LinkedCellsContainer::LinkedCellsContainer(const std::array<double, 3>& size, double cutoff_radius, int n)
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
                } else if (cx == 0 || cx == domain_num_cells[0] - 1 || cy == 0 || cy == domain_num_cells[1] - 1 || cz == 0 ||
                           cz == domain_num_cells[2] - 1) {
                    Cell newCell(Cell::CellType::BOUNDARY);
                    cells.push_back(newCell);
                    boundary_cell_references.push_back(&cells.back());
                } else {
                    Cell newCell(Cell::CellType::INNER);
                    cells.push_back(newCell);
                    inner_cell_references.push_back(&cells.back());
                }
            }
        }
    }
}

void LinkedCellsContainer::addParticle(const Particle& p) {
    Cell* cell = particlePosToCell(p.getX());

    if (cell == nullptr) {
        Logger::logger->error("Particle to insert is out of bounds");
        return;
    }

    particles.push_back(p);
    cell->addParticleReference(&particles.back());
}

void LinkedCellsContainer::addParticle(Particle&& p) {
    Cell* cell = particlePosToCell(p.getX());

    if (cell == nullptr) {
        Logger::logger->error("Particle to insert is out of bounds");
        return;
    }

    particles.push_back(std::move(p));
    cell->addParticleReference(&particles.back());
}

void LinkedCellsContainer::applyPairwiseForces(const std::vector<std::unique_ptr<ForceSource>>& force_sources) {
    std::cout << "applyPairwiseForces(const std::vector<std::unique_ptr<ForceSource>>& force_sources) not implemented" << std::endl;
}

void LinkedCellsContainer::reserve(size_t n) { particles.reserve(n); }

size_t LinkedCellsContainer::size() const { return particles.size(); }

Particle& LinkedCellsContainer::operator[](int i) { return particles[i]; }

std::vector<Particle>::iterator LinkedCellsContainer::begin() { return particles.begin(); }

std::vector<Particle>::iterator LinkedCellsContainer::end() { return particles.end(); }

std::vector<Particle>::const_iterator LinkedCellsContainer::begin() const { return particles.begin(); }

std::vector<Particle>::const_iterator LinkedCellsContainer::end() const { return particles.end(); }

const std::array<double, 3>& LinkedCellsContainer::getDomainSize() const { return domain_size; }

double LinkedCellsContainer::getCutoffRadius() const { return cutoff_radius; }

const std::vector<Cell>& LinkedCellsContainer::getCells() { return cells; }

const std::vector<Cell*>& LinkedCellsContainer::getBoundaryCells() const { return boundary_cell_references; }

const std::array<double, 3>& LinkedCellsContainer::getCellSize() const { return cell_size; }

const std::array<int, 3>& LinkedCellsContainer::getDomainNumCells() const { return domain_num_cells; }

int LinkedCellsContainer::cellCoordToCellIndex(int cx, int cy, int cz) const {
    if (cx < -1 || cx > domain_num_cells[0] || cy < -1 || cy > domain_num_cells[1] || cz < -1 || cz > domain_num_cells[2]) {
        return -1;
    }
    return (cx + 1) * (domain_num_cells[1] + 2) * (domain_num_cells[2] + 2) + (cy + 1) * (domain_num_cells[2] + 2) + (cz + 1);
}

Cell* LinkedCellsContainer::particlePosToCell(const std::array<double, 3>& pos) { return particlePosToCell(pos[0], pos[1], pos[2]); }

Cell* LinkedCellsContainer::particlePosToCell(double x, double y, double z) {
    int cx = static_cast<int>(std::floor(x / cell_size[0]));
    int cy = static_cast<int>(std::floor(y / cell_size[1]));
    int cz = static_cast<int>(std::floor(z / cell_size[2]));

    int cell_index = cellCoordToCellIndex(cx, cy, cz);
    if (cell_index == -1) {
        return nullptr;
    }

    return &cells[cell_index];
}