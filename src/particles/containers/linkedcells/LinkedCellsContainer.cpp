#include "LinkedCellsContainer.h"

#include <cmath>

#include "cells/Cell.h"
#include "io/logger/Logger.h"
#include "physics/LennardJonesForce.h"
#include "utils/ArrayUtils.h"

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
           particle_index >= static_cast<int>(cells.at(cell_index)->getParticleReferences().size())) {
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
    return *(cells.at(cell_index)->getParticleReferences().at(particle_index));
}

Particle* LinkedCellsContainer::BoundaryIterator::operator->() const {
    if (cell_index == -1 || particle_index == -1) return nullptr;
    return cells.at(cell_index)->getParticleReferences().at(particle_index);
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
LinkedCellsContainer::LinkedCellsContainer(const std::array<double, 3>& _domain_size, double _cutoff_radius,
                                           const std::array<BoundaryCondition, 6>& _boundary_types, int _n)
    : domain_size(_domain_size), cutoff_radius(_cutoff_radius), boundary_types(_boundary_types) {
    domain_num_cells = {static_cast<int>(std::ceil(_domain_size[0] / cutoff_radius)),
                        static_cast<int>(std::ceil(_domain_size[1] / cutoff_radius)),
                        static_cast<int>(std::ceil(_domain_size[2] / cutoff_radius))};

    cell_size = {_domain_size[0] / domain_num_cells[0], _domain_size[1] / domain_num_cells[1], _domain_size[2] / domain_num_cells[2]};

    // reserve the memory for the cells
    cells.reserve((domain_num_cells[0] + 2) * (domain_num_cells[1] + 2) * (domain_num_cells[2] + 2));

    // create the cells with the correct cell-type and add them to the cells vector and the corresponding cell reference vector
    initCells();

    // add the neighbour references to the cells
    initCellNeighbourReferences();

    // reserve the memory for the particles to prevent reallocation during insertion
    particles.reserve(_n);

    Logger::logger->info("Created LinkedCellsContainer with {} domain cells (of which {} are at the boundary) and {} halo cells",
                         domain_cell_references.size(), boundary_cell_references.size(), halo_cell_references.size());
    Logger::logger->info("Cells per dimension: [{}, {}, {}]", domain_num_cells[0], domain_num_cells[1], domain_num_cells[2]);
    Logger::logger->info("Calculated cell size: [{}, {}, {}]", cell_size[0], cell_size[1], cell_size[2]);
}

void LinkedCellsContainer::addParticle(const Particle& p) {
    Cell* cell = particlePosToCell(p.getX());

    if (cell == nullptr) {
        Logger::logger->error("Particle to insert is out of bounds");
        return;
    }
    if (cell->getCellType() == Cell::CellType::HALO) {
        Logger::logger->warn("Particle to insert is in halo cell");
        return;
    }

    size_t old_capacity = particles.capacity();
    particles.push_back(p);

    if (old_capacity != particles.capacity()) {
        updateCellsParticleReferences();
    } else {
        cell->addParticleReference(&particles.back());
    }
}

void LinkedCellsContainer::addParticle(Particle&& p) {
    Cell* cell = particlePosToCell(p.getX());

    if (cell == nullptr) {
        Logger::logger->warn("Particle to insert is out of bounds");
        return;
    }
    if (cell->getCellType() == Cell::CellType::HALO) {
        Logger::logger->warn("Particle to insert is in halo cell");
        return;
    }

    size_t old_capacity = particles.capacity();
    particles.push_back(std::move(p));

    if (old_capacity != particles.capacity()) {
        updateCellsParticleReferences();
    } else {
        cell->addParticleReference(&particles.back());
    }
}

void LinkedCellsContainer::applyPairwiseForces(const std::vector<std::unique_ptr<ForceSource>>& force_sources) {
    // remove all particles in the halo cells from the particles vector
    deleteHaloParticles();

    // update the particle references in the cells in case the particles have moved
    updateCellsParticleReferences();

    // apply the boundary conditions
    applyReflectiveBoundaryConditions();

    // clear the already influenced by vector in the cells
    // this is needed to prevent the two cells from affecting each other twice
    // since newtons third law is used
    for (Cell* cell : domain_cell_references) {
        cell->clearAlreadyInfluencedBy();
    }

    for (Cell* cell : occupied_cells_references) {
        // skip halo cells
        if (cell->getCellType() == Cell::CellType::HALO) continue;

        for (auto it1 = cell->getParticleReferences().begin(); it1 != cell->getParticleReferences().end(); ++it1) {
            Particle* p = *it1;
            // calculate the forces between the particle and the particles in the same cell
            // uses direct sum with newtons third law
            for (auto it2 = (it1 + 1); it2 != cell->getParticleReferences().end(); ++it2) {
                Particle* q = *it2;
                std::array<double, 3> total_force{0, 0, 0};
                for (auto& force : force_sources) {
                    total_force = total_force + force->calculateForce(*p, *q);
                }
                p->setF(p->getF() + total_force);
                q->setF(q->getF() - total_force);
            }

            // calculate the forces between the particle and the particles in the neighbour cells
            for (Cell* neighbour : cell->getNeighbourReferences()) {
                if (cell->getAlreadyInfluencedBy().contains(neighbour)) continue;

                for (Particle* neighbour_particle : neighbour->getParticleReferences()) {
                    if (ArrayUtils::L2Norm(p->getX() - neighbour_particle->getX()) > cutoff_radius) continue;

                    for (const std::unique_ptr<ForceSource>& force_source : force_sources) {
                        std::array<double, 3> force = force_source->calculateForce(*p, *neighbour_particle);
                        p->setF(p->getF() + force);
                        neighbour_particle->setF(neighbour_particle->getF() - force);
                    }
                }

                neighbour->addAlreadyInfluencedBy(cell);
            }
        }
    }
}

void LinkedCellsContainer::reserve(size_t n) {
    Logger::logger->debug("Reserving space for {} particles", n);

    size_t old_capacity = particles.capacity();
    particles.reserve(n);

    if (old_capacity != particles.capacity()) {
        updateCellsParticleReferences();
    }
}

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
        Logger::logger->error("Particle not in cells");
        return nullptr;
    }

    return &cells[cell_index];
}

/*
    Private methods of the LinkedCellsContainer
*/

void LinkedCellsContainer::initCells() {
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
                    domain_cell_references.push_back(&cells.back());

                    if (cx == 0) {
                        left_boundary_cell_references.push_back(&cells.back());
                    }
                    if (cx == domain_num_cells[0] - 1) {
                        right_boundary_cell_references.push_back(&cells.back());
                    }
                    if (cy == 0) {
                        bottom_boundary_cell_references.push_back(&cells.back());
                    }
                    if (cy == domain_num_cells[1] - 1) {
                        top_boundary_cell_references.push_back(&cells.back());
                    }
                    if (cz == 0) {
                        back_boundary_cell_references.push_back(&cells.back());
                    }
                    if (cz == domain_num_cells[2] - 1) {
                        front_boundary_cell_references.push_back(&cells.back());
                    }
                } else {
                    Cell newCell(Cell::CellType::INNER);
                    cells.push_back(newCell);
                    domain_cell_references.push_back(&cells.back());
                }
            }
        }
    }
}

void LinkedCellsContainer::initCellNeighbourReferences() {
    // Loop through each cell according to their cell coordinates
    for (int cx = -1; cx < domain_num_cells[0] + 1; ++cx) {
        for (int cy = -1; cy < domain_num_cells[1] + 1; ++cy) {
            for (int cz = -1; cz < domain_num_cells[2] + 1; ++cz) {
                Cell& cell = cells.at(cellCoordToCellIndex(cx, cy, cz));

                // Loop through each of the current cells neighbour cells according to their cell coordinates
                // except the current cell itself
                for (int dx = -1; dx < 2; ++dx) {
                    for (int dy = -1; dy < 2; ++dy) {
                        for (int dz = -1; dz < 2; ++dz) {
                            if (dx == 0 && dy == 0 && dz == 0) continue;

                            // Get the cell index of the current neighbour cell
                            int cell_index = cellCoordToCellIndex(cx + dx, cy + dy, cz + dz);

                            // If the neighbour cell would be out of bounds, skip it
                            if (cell_index == -1) continue;

                            // Add the neighbour to the current cells neighbour references
                            Cell& curr_neighbour = cells.at(cell_index);
                            cell.addNeighbourReference(&curr_neighbour);
                        }
                    }
                }
            }
        }
    }
}

void LinkedCellsContainer::updateCellsParticleReferences() {
    // clear the particle references in the cells
    for (Cell& cell : cells) {
        cell.clearParticleReferences();
    }

    // clear the set of used cells
    occupied_cells_references.clear();

    // add the particle references to the cells
    for (Particle& p : particles) {
        Cell* cell = particlePosToCell(p.getX());

        if (cell == nullptr) {
            Logger::logger->error("Particle reference update: Particle is out of bounds");
            continue;
        }

        occupied_cells_references.insert(cell);
        cell->addParticleReference(&p);
    }
}

void LinkedCellsContainer::deleteHaloParticles() {
    for (Cell* cell : halo_cell_references) {
        for (Particle* p : cell->getParticleReferences()) {
            particles.erase(std::find(particles.begin(), particles.end(), *p));
        }
    }
}

void LinkedCellsContainer::applyReflectiveBoundaryConditions() {
    if (boundary_types[0] == BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : left_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = p->getX()[0];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, BoundarySide::LEFT));
            }
        }
    }

    if (boundary_types[1] == BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : right_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = domain_size[0] - p->getX()[0];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, BoundarySide::RIGHT));
            }
        }
    }

    if (boundary_types[2] == BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : bottom_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = p->getX()[1];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, BoundarySide::BOTTOM));
            }
        }
    }

    if (boundary_types[3] == BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : top_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = domain_size[1] - p->getX()[1];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, BoundarySide::TOP));
            }
        }
    }

    if (boundary_types[4] == BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : back_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = p->getX()[2];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, BoundarySide::BACK));
            }
        }
    }

    if (boundary_types[5] == BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : front_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = domain_size[2] - p->getX()[2];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, BoundarySide::FRONT));
            }
        }
    }
}

std::array<double, 3> LinkedCellsContainer::calculateReflectiveBoundaryForce(Particle& p, double distance, BoundarySide side) {
    Particle ghost_particle = Particle(p.getX() - std::array<double, 3>{2 * distance, 0, 0}, {0, 0, 0}, p.getM());
    LennardJonesForce force = LennardJonesForce();

    auto force_vector_left_side = force.calculateForce(p, ghost_particle);

    switch (side) {
        case BoundarySide::LEFT:
            return force_vector_left_side;
        case BoundarySide::RIGHT:
            return {-force_vector_left_side[0], 0, 0};
        case BoundarySide::BOTTOM:
            return {0, force_vector_left_side[0], 0};
        case BoundarySide::TOP:
            return {0, -force_vector_left_side[0], 0};
        case BoundarySide::BACK:
            return {0, 0, force_vector_left_side[0]};
        case BoundarySide::FRONT:
            return {0, 0, -force_vector_left_side[0]};
    }

    Logger::logger->error("Faulty reflective boundary condition");
    return {0, 0, 0};
}
