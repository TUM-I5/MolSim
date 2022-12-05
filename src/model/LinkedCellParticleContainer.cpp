/*
 * LinkedCellParticleContainer.cpp
 *
 * Created: 30.11.2022
 * Author:  marquardt
 */

#include "LinkedCellParticleContainer.h"
#include "../utils/ArrayUtils.h"

LinkedCellParticleContainer::LinkedCellParticleContainer(double reflectingDistance, double cutoff, std::array<double, 3> &domain, std::array<BoundaryCondition,6> &domainBoundaries) {
    //TODO: Logging, initialization of variables, reserve vector space, compute cell number & size
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("LinkedCellParticleContainer generated!");

    //get cutoff & domain size & boundary conditions (for whole domain) from program parameters?

    _reflectingDistance = reflectingDistance;
    _domain = domain;
    _cutoff = cutoff;

    initializeCells(domainBoundaries);

}
LinkedCellParticleContainer::~LinkedCellParticleContainer() {
     _memoryLogger->info("LinkedCellParticleContainer destructed!");
}

const void LinkedCellParticleContainer::initializeCells(std::array<BoundaryCondition,6> &domainBoundaries) {
    int cellsX = static_cast<int>(std::floor(_domain[0] / _cutoff));
    int cellsY = static_cast<int>(std::floor(_domain[1] / _cutoff));
    int cellsZ = static_cast<int>(std::floor(_domain[2] / _cutoff));

    //for 2D we need exactly one layer, which is not guaranteed above
    if (cellsZ == 0) {
        cellsZ = 1;
    }
    _numCells = {cellsX, cellsY, cellsZ};

    
    double sizeX = _domain[0] / _numCells[0];
    double sizeY = _domain[1] / _numCells[1];
    double sizeZ = _domain[2] / _numCells[2];

    //cell size has to be at least 1, otherwise we get problems with division by 0
    if (sizeZ == 0) {
        sizeZ = 1;
    }
    _cellSize = {sizeX, sizeY, sizeZ};

    //reserve space for cells
     _cellVector.reserve(_numCells[0] * _numCells[1] * _numCells[2]);

    //initialize cells according to boundary conditions
    for (int i = 0; i < _numCells[0] * _numCells[1] * _numCells[2]; i++) {
        BoundaryCondition init = BoundaryCondition::Outflow;
        std::array<BoundaryCondition, 6> cellBoundaries = {init, init, init, init, init, init};
        CellType type = CellType::InnerCell;
        //cell at left boundary
        if (i % _numCells[0] == 0) {
            cellBoundaries[0] = domainBoundaries[0];
            type = CellType::BoundaryCell;
        }
        //cell at right boundary
        if (i % _numCells[0] == _numCells[0]-1) {
            cellBoundaries[1] = domainBoundaries[1];
            type = CellType::BoundaryCell;
        }
        //cell at bottom boundary
        if ((i / _numCells[0]) % _numCells[1] == 0) {
            cellBoundaries[2] = domainBoundaries[2];
            type = CellType::BoundaryCell;
        }
        //cell at top boundary
        if ((i / _numCells[0]) % _numCells[1] == _numCells[1] - 1) {
            cellBoundaries[3] = domainBoundaries[3];
            type = CellType::BoundaryCell;
        }
        //cell at front boundary
        if ((i / (_numCells[0] * _numCells[1])) % _numCells[2] == 0) {
            cellBoundaries[4] = domainBoundaries[4];
            type = CellType::BoundaryCell;
        }
        //cell at back boundary
        if ((i / (_numCells[0] * _numCells[1])) % _numCells[2] == _numCells[2] - 1) {
            cellBoundaries[5] = domainBoundaries[5];
            type = CellType::BoundaryCell;
        }
        _cellVector.emplace(_cellVector.begin() + i, type, cellBoundaries);
    }

}

const int LinkedCellParticleContainer::computeCellIdx(Particle &p) {
    int cell_idx_x = static_cast<int>(std::floor(p.getX()[0] / _cellSize[0]));  
    int cell_idx_y = static_cast<int>(std::floor(p.getX()[1] / _cellSize[1]));
    int cell_idx_z = static_cast<int>(std::floor(p.getX()[2] / _cellSize[2]));

    //one of the cell indices out of domain bounds
    if (cell_idx_x < 0 || cell_idx_x >= _numCells[0] || cell_idx_y < 0 || cell_idx_y >= _numCells[1] || cell_idx_z < 0 || cell_idx_z >= _numCells[2]) {
        return -1;
    }
    else {
        return cell_idx_z * _numCells[1] * _numCells[0] + cell_idx_y * _numCells[0] + cell_idx_x;
    }
}

const void LinkedCellParticleContainer::updateCells() {
    //update counter before insert operation because it influences its behavior
    for (auto &c : _cellVector) {
        c.updateInvalidCounter();
    }

    for (auto &p : _activeParticleVector) {
        if (p.getInvalid()) {
            _cellVector[p.getCellIdx()].insertParticle(&p);
            p.setInvalid(false);
        }
    }
}


const void LinkedCellParticleContainer::addParticle(std::array<double, 3> &x, std::array<double, 3> &v, double &m) {
    _activeParticleVector.emplace_back(x,v,m);
    Particle &p = _activeParticleVector.back();
    int cell_idx = computeCellIdx(p);
    p.setCellIdx(cell_idx);
    _cellVector[cell_idx].insertParticle(&p);

}

const void LinkedCellParticleContainer::iterateParticles(std::function<void(Particle &)> f) {
    bool restructure = false;
    bool restructureAll = false;

    for (auto &p : _activeParticleVector) {
        f(p);
        int new_cell_idx = computeCellIdx(p); 
        if (new_cell_idx != p.getCellIdx()) {
            p.setCellIdx(new_cell_idx);

            //particle still in domain
            if (p.getCellIdx() >= 0 && p.getCellIdx() < static_cast<int>(_cellVector.size())) {
                p.setInvalid(true);
                restructure = true;
            }
            //particle in halo
            else {
                p.setHalo(true);
                _haloParticleVector.push_back(p);
                restructureAll = true;
            }
        }
    }

    if(restructureAll) {
        //remove halo particles
        _activeParticleVector.erase(std::remove_if(_activeParticleVector.begin(), _activeParticleVector.end(), [](Particle &p) { return p.getHalo(); }), _activeParticleVector.end());

        //through deletion of particles references become invalid
        for (auto &p : _activeParticleVector) {
            p.setInvalid(true);
        }
        updateCells();
    }
    else if(restructure) {
        updateCells();
    }
    
}

const void LinkedCellParticleContainer::iterateParticleInteractions(std::function<void(Particle &, Particle &)> f) {

    //to implement Newton's 3rd law only calculate force interaction with neighboring cells with a higher index in 1D cell vector
    for (long unsigned int i = 0; i < _cellVector.size(); i++) {
        ParticleCell &curr_cell = _cellVector[i];

        //interaction in same cell, already implementing Newton's 3rd law
        curr_cell.iterateParticlePairs(f);

        //interaction with other cells, maybe find formula for step_size so it only takes adjacent cells
        for (long unsigned int j = i + 1; j < _cellVector.size(); j++) {
            //TODO: condition for adjacent cell 
            for (auto p1 : curr_cell.getCellParticles()) {
                for (auto p2 : _cellVector[j].getCellParticles()) {
                    if (ArrayUtils::L2Norm(p1->getX() - p2->getX()) <= _cutoff) {
                        f(*p1,*p2);
                    }
                }
            }
        }

        //boundary conditions
        if (curr_cell.getType() == CellType::BoundaryCell) {
            for (int b = 0; b < 6; b++) {
                if (curr_cell.getBoundaries()[b] == BoundaryCondition::Reflecting) {
                    reflectingBoundary(curr_cell.getCellParticles(), b, f);
                }
            }
        }
    }
        
}

const void LinkedCellParticleContainer::reflectingBoundary(std::vector<Particle *> &particles, int boundary_idx, std::function<void(Particle &, Particle &)> f) {

    //check distance of particle to reflecting boundary, if it is close enough apply reflecting force
    //reflecting force calculated through counter particle on the opposite side of the borader (same distance)
    Particle counterParticle = Particle();

    for (auto p : particles) {
        switch(boundary_idx) {
            //left boundary, x-coordinate is 0
            case 0:
                if (p->getX()[0] <= _reflectingDistance) {
                    std::array<double, 3> counterX = {(-1) * p->getX()[0], p->getX()[1], p->getX()[2]};
                    counterParticle.setX(counterX);
                    f(*p, counterParticle);
                }
                break;
            //right boundary, x-coordinate is domain size in x-direction
            case 1:
                if (_domain[0] - p->getX()[0] <= _reflectingDistance) {
                    std::array<double, 3> counterX = {_domain[0] + _domain[0] - p->getX()[0], p->getX()[1], p->getX()[2]};
                    counterParticle.setX(counterX);
                    f(*p, counterParticle);
                }
                break;
            //lower boundary, y-coordinate is 0
            case 2:
                if (p->getX()[1] <= _reflectingDistance) {
                    std::array<double, 3> counterX = {p->getX()[0], (-1) * p->getX()[1], p->getX()[2]};
                    counterParticle.setX(counterX);
                    f(*p, counterParticle);
                }
                break;
            //upper boundary, y-coordinate is domain size in y-direction
            case 3:
                if (_domain[1] - p->getX()[1] <= _reflectingDistance) {
                    std::array<double, 3> counterX = {p->getX()[0], _domain[1] + _domain[1] - p->getX()[1], p->getX()[2]};
                    counterParticle.setX(counterX);
                    f(*p, counterParticle);
                }
                break;
            //front boundary, z-coordinate is 0
            case 4:
                if (p->getX()[2] <= _reflectingDistance) {
                    std::array<double, 3> counterX = {p->getX()[0], p->getX()[1], (-1) * p->getX()[2]};
                    counterParticle.setX(counterX);
                    f(*p, counterParticle);
                }
                break;
            //back boundary, z-coordinate is domain size in z-direction
            case 5:
                if (_domain[2] - p->getX()[2] <= _reflectingDistance) {
                    std::array<double, 3> counterX = {p->getX()[0], p->getX()[1], _domain[2] + _domain[2] - p->getX()[2]};
                    counterParticle.setX(counterX);
                    f(*p, counterParticle);
                }
                break;
        }
    }
}

const void LinkedCellParticleContainer::reserveMemoryForParticles(int numberOfParticles)
{
    // reserving extra space before new particles are added. Push_back can then be executed without resizing
    int newLength = numberOfParticles + _activeParticleVector.size();
    _activeParticleVector.reserve(newLength);

    //reserving extra space in each cell with the mean value of particles per cell
    for (auto cell : _cellVector) {
        cell.reserveMemory(numberOfParticles / (_numCells[0] * _numCells[1] * _numCells[2]));
    }
}

const void LinkedCellParticleContainer::resetParticles() { 
    _activeParticleVector.clear();
    _haloParticleVector.clear();
    for (auto cell : _cellVector) {
        cell.clearCell();
    }
}

const void LinkedCellParticleContainer::clearHalo() { _haloParticleVector.clear(); }

const int LinkedCellParticleContainer::size() const { return _activeParticleVector.size(); }

std::vector<ParticleCell> &LinkedCellParticleContainer::getCells() { return _cellVector; }

std::vector<Particle> &LinkedCellParticleContainer::getHaloParticles() { return _haloParticleVector; }

std::vector<Particle> &LinkedCellParticleContainer::getActiveParticles() { return _activeParticleVector; }


