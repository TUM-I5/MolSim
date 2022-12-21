/*
 * LinkedCellParticleContainer.cpp
 *
 * Created: 30.11.2022
 * Author:  marquardt
 */

#include "LinkedCellParticleContainer.h"
#include "../utils/ArrayUtils.h"
#include "../utils/PContainer.h"

#include <iostream>
#include <string>

LinkedCellParticleContainer::LinkedCellParticleContainer(double sigma, double cutoff, std::array<double, 3> &domain, std::array<BoundaryCondition, 6> &domainBoundaries)
{
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("LinkedCellParticleContainer generated!");
    _simulationLogger = spdlog::get("simulation_logger");

    _domain = domain;
    _cutoff = cutoff;

    initializeCells(domainBoundaries);
}
LinkedCellParticleContainer::~LinkedCellParticleContainer()
{
    _memoryLogger->info("LinkedCellParticleContainer destructed!");
}

const void LinkedCellParticleContainer::initializeCells(std::array<BoundaryCondition, 6> &domainBoundaries)
{
    int cellsX = static_cast<int>(std::floor(_domain[0] / _cutoff));
    int cellsY = static_cast<int>(std::floor(_domain[1] / _cutoff));
    int cellsZ = static_cast<int>(std::floor(_domain[2] / _cutoff));

    // for 2D we need exactly one layer, which is not guaranteed above
    if (cellsZ == 0)
        cellsZ = 1;
    
    //add surrounding halo cells
    _numCells = {cellsX+2, cellsY+2, cellsZ+2};

    double sizeX = _domain[0] / cellsX;
    double sizeY = _domain[1] / cellsY;
    double sizeZ = _domain[2] / cellsZ;

    // cell size has to be at least 1, otherwise we get problems with division by 0
    if (sizeZ == 0)
    {
        sizeZ = 1;
    }
    _cellSize = {sizeX, sizeY, sizeZ};

    // reserve space for cells
    _cellVector.reserve(_numCells[0] * _numCells[1] * _numCells[2]);

    // initialize cells according to boundary conditions
    for (int i = 0; i < _numCells[0] * _numCells[1] * _numCells[2]; i++)
    {
        BoundaryCondition init = BoundaryCondition::Outflow;
        std::array<BoundaryCondition, 6> cellBoundaries = {init, init, init, init, init, init};
        CellType type = CellType::InnerCell;

        std::array<int,3> index_3D = PContainer::convert1DTo3D(i, _numCells);

        //halo cell
        if (index_3D[0] == 0 || index_3D[0] == _numCells[0]-1 
            || index_3D[1] == 0 || index_3D[1] == _numCells[1]-1
            || index_3D[2] == 0 || index_3D[2] == _numCells[2]-1) 
        {
            type = CellType::HaloCell;
        }

        else {
            // cell at left boundary
            if (index_3D[0] == 1)
            {
                cellBoundaries[0] = domainBoundaries[0];
                type = CellType::BoundaryCell;
            }
            // cell at right boundary
            if (index_3D[0] == _numCells[0] - 2)
            {
                cellBoundaries[1] = domainBoundaries[1];
                type = CellType::BoundaryCell;
            }
            // cell at bottom boundary
            if (index_3D[1] == 1)
            {
                cellBoundaries[2] = domainBoundaries[2];
                type = CellType::BoundaryCell;
            }
            // cell at top boundary
            if (index_3D[1] == _numCells[1] - 2)
            {
                cellBoundaries[3] = domainBoundaries[3];
                type = CellType::BoundaryCell;
            }
            // cell at front boundary
            if (index_3D[2] == 1)
            {
                cellBoundaries[4] = domainBoundaries[4];
                type = CellType::BoundaryCell;
            }
            // cell at back boundary
            if (index_3D[2] == _numCells[2] - 2)
            {
                cellBoundaries[5] = domainBoundaries[5];
                type = CellType::BoundaryCell;
            }
        }
        _cellVector.emplace(_cellVector.begin() + i, type, cellBoundaries);
    }

    // set neighboring cell indices (which are higher than the current cell index)
    for (unsigned int i = 0; i < _cellVector.size(); i++)
    {   
        if (_cellVector[i].getType() != CellType::HaloCell) {
            std::vector<int> domainNeighbours = PContainer::getDomainNeighboursNewton(i, _numCells);
            _cellVector[i].setNeighbours(domainNeighbours);
            std::vector<int> haloNeighbours = PContainer::getHaloNeighbours(i, _numCells);
            _cellVector[i].setHaloNeighbours(haloNeighbours);
        }
    }
}

const int LinkedCellParticleContainer::computeCellIdx(Particle &p)
{
    //+1 because of halo cells
    int cell_idx_x = static_cast<int>(std::floor(p.getX()[0] / _cellSize[0])) + 1;
    int cell_idx_y = static_cast<int>(std::floor(p.getX()[1] / _cellSize[1])) + 1;
    int cell_idx_z = static_cast<int>(std::floor(p.getX()[2] / _cellSize[2])) + 1;

    return cell_idx_z * _numCells[1] * _numCells[0] + cell_idx_y * _numCells[0] + cell_idx_x;
}

const void LinkedCellParticleContainer::updateCells()
{
    for (auto &c : _cellVector)
    {
        c.removeInvalid();
    }

    for (auto &p : _activeParticleVector)
    {
        if (p.getInvalid() && !p.getHalo())
        {
            _cellVector[p.getCellIdx()].insertParticle(&p);
            p.setInvalid(false);
        }
    }
}

const void LinkedCellParticleContainer::rebuildCells()
{
    for (auto &c : _cellVector)
    {
        c.clearCell();
    }

    for (auto &p : _activeParticleVector)
    {
        if (!p.getHalo())
        {
            _cellVector[p.getCellIdx()].insertParticle(&p);
            p.setInvalid(false);
        }
    }
}

const void LinkedCellParticleContainer::addParticle(std::array<double, 3> &x, std::array<double, 3> &v, double &m, double &epsilon, double &sigma)
{
    if (x[0] >= 0 && x[0] < _domain[0] && x[1] >= 0 && x[1] < _domain[1] && x[2] >= 0 && x[2] < _domain[2])
    {
        if (_activeParticleVector.capacity() == _activeParticleVector.size())
        {
            _activeParticleVector.emplace_back(x, v, m, epsilon, sigma);
            rebuildCells();
        }
        else
        {
            _activeParticleVector.emplace_back(x, v, m, epsilon, sigma);
            Particle &p = _activeParticleVector.back();
            int cell_idx = computeCellIdx(p);
            p.setCellIdx(cell_idx);
            _cellVector[cell_idx].insertParticle(&p);
        }
    }
}

const void LinkedCellParticleContainer::addParticle(std::array<double, 3> &x, std::array<double, 3> &v, double &m, double &epsilon, double &sigma, int &type)
{
    if (x[0] >= 0 && x[0] < _domain[0] && x[1] >= 0 && x[1] < _domain[1] && x[2] >= 0 && x[2] < _domain[2])
    {
        if (_activeParticleVector.capacity() == _activeParticleVector.size())
        {
            _activeParticleVector.emplace_back(x, v, m, epsilon, sigma, type);
            rebuildCells();
        }
        else
        {
            _activeParticleVector.emplace_back(x, v, m, epsilon, sigma, type);
            Particle &p = _activeParticleVector.back();
            int cell_idx = computeCellIdx(p);
            p.setCellIdx(cell_idx);
            _cellVector[cell_idx].insertParticle(&p);
        }
    }
}

const void LinkedCellParticleContainer::addParticle(std::array<double, 3> &x, std::array<double, 3> &v, std::array<double, 3> &f, std::array<double, 3> &old_f, double &m, double &epsilon, double &sigma, int &type)
{
    if (x[0] >= 0 && x[0] < _domain[0] && x[1] >= 0 && x[1] < _domain[1] && x[2] >= 0 && x[2] < _domain[2])
    {
        _activeParticleVector.emplace_back(x, v, f, old_f, m, epsilon, sigma, type);
        Particle &p = _activeParticleVector.back();
        int cell_idx = computeCellIdx(p);
        p.setCellIdx(cell_idx);
        _cellVector[cell_idx].insertParticle(&p);
    }
}

const void LinkedCellParticleContainer::iterateParticles(std::function<void(Particle &)> f)
{
    bool restructure = false;
    bool restructureAll = false;

    for (auto &p : _activeParticleVector)
    {
        f(p);
        int new_cell_idx = computeCellIdx(p);
        if (new_cell_idx != p.getCellIdx())
        {
            //particle left to halo
            if (_cellVector[new_cell_idx].getType() == CellType::HaloCell) {
                int crossedBoundary = PContainer::crossedBoundary(p.getCellIdx(), new_cell_idx, _numCells);
                //particle crossed periodic boundary, reappears on other side of domain
                if (_cellVector[p.getCellIdx()].getBoundaries()[crossedBoundary] == BoundaryCondition::Periodic) {
                    p.setX(mirroredPosition(p, crossedBoundary));
                    p.setCellIdx(computeCellIdx(p));
                    p.setInvalid(true);
                    restructure = true;
                    _simulationLogger->debug("Particle reappears at opposite boundary");
                }
                //particle crossed outflow boundary
                else
                {
                    p.setHalo(true);
                    p.setInvalid(true);
                    p.setCellIdx(new_cell_idx);
                    _haloParticleVector.push_back(p);
                    restructureAll = true;
                    _simulationLogger->debug("Particle left to halo");
                }
            }
            // particle still in domain
            else
            {
                p.setInvalid(true);
                p.setCellIdx(new_cell_idx);
                restructure = true;
                _simulationLogger->debug("Particle left to another cell");
            }
            
        }
    }

    if (restructureAll)
    {
        // remove halo particles
        _activeParticleVector.erase(std::remove_if(_activeParticleVector.begin(), _activeParticleVector.end(), [](Particle &p)
                                                   { return p.getHalo(); }),
                                    _activeParticleVector.end());

        rebuildCells();
    }
    else if (restructure)
    {
        updateCells();
    }
}

const void LinkedCellParticleContainer::iterateParticleInteractions(std::function<void(Particle &, Particle &)> f)
{   
    //boundary conditions
    clearHalo();
    for (auto &cell : _cellVector) {
        if (cell.getType() == CellType::BoundaryCell)
        {
            for (int b = 0; b < 6; b++)
            {
                if (cell.getBoundaries()[b] == BoundaryCondition::Reflecting)
                {
                    reflectingBoundary(*cell.getCellParticles(), b, f);
                }
                else if (cell.getBoundaries()[b] == BoundaryCondition::Periodic) 
                {
                    initGhostParticles(*cell.getCellParticles(), b);
                }
            }
        }
    }

    // to implement Newton's 3rd law only calculate force interaction with neighboring cells with a higher index in 1D cell vector
    for (long unsigned int i = 0; i < _cellVector.size(); i++)
    {
        ParticleCell &curr_cell = _cellVector[i];

        //Halo Cells should not compute interactions in themselves
        //Interactions with boundary cells are handled when they are called
        if (curr_cell.getType() == CellType::HaloCell) 
            continue;

        // interaction in same cell, already implementing Newton's 3rd law
        curr_cell.iterateParticlePairs(f, _cutoff);
        
        std::vector<int> allNeighbors{curr_cell.getNeighbours()};
        allNeighbors.insert(allNeighbors.end(), curr_cell.getHaloNeighbours().begin(), curr_cell.getHaloNeighbours().end());

        // interaction with neighboring cells (with higher index) and halo cells
        for (int j : allNeighbors)
        {
            for (auto p1 : *curr_cell.getCellParticles())
            {
                for (auto p2 : *_cellVector[j].getCellParticles())
                {
                    if (ArrayUtils::L2Norm(p1->getX() - p2->getX()) <= _cutoff)
                    {
                        f(*p1, *p2); // no invalid check necessary because getCellParticles returns only valid particles
                    }
                }
            }
        }

    }
}

std::array<double,3> LinkedCellParticleContainer::mirroredPosition(Particle &p, int boundary_idx) {
    std::array<double, 3> newX;
    switch(boundary_idx) {
            //left boundary, has to be mirrored to right halo
            case 0:
                newX = {p.getX()[0] + _domain[0], p.getX()[1], p.getX()[2]};
                break;
            //right boundary, has to be mirrored to left halo
            case 1:
                newX = {p.getX()[0] - _domain[0], p.getX()[1], p.getX()[2]};
                break;
            //lower boundary, has to be mirrored to upper halo
            case 2:
                newX = {p.getX()[0], p.getX()[1] + _domain[1], p.getX()[2]};
                break;
            //upper boundary, has to be mirrored to lower halo
            case 3:
                newX = {p.getX()[0], p.getX()[1] - _domain[1], p.getX()[2]};
                break;
            //front boundary, has to be mirrored to back halo
            case 4:
                newX = {p.getX()[0], p.getX()[1], p.getX()[2] + _domain[2]};
                break;
            //back boundary, has to be mirrored to front halo
            case 5:
                newX = {p.getX()[0], p.getX()[1], p.getX()[2] - _domain[2]};
                break;
    }
    return newX;
}

const void LinkedCellParticleContainer::initGhostParticles(std::vector<Particle *> &particles, int boundary_idx) {
    for (auto p : particles) {
        std::array<double, 3> newX = mirroredPosition(*p, boundary_idx);
        _haloParticleVector.emplace_back(*p);
        Particle &ghostParticle = _haloParticleVector.back();
        ghostParticle.setX(newX);
        int idx = computeCellIdx(ghostParticle);
        ghostParticle.setCellIdx(idx);
        _cellVector[idx].insertParticle(&ghostParticle);
    }
    
}

const void LinkedCellParticleContainer::reflectingBoundary(std::vector<Particle *> &particles, int boundary_idx, std::function<void(Particle &, Particle &)> f)
{

    // check distance of particle to reflecting boundary, if it is close enough apply reflecting force
    // reflecting force calculated through counter particle on the opposite side of the borader (same distance)
    Particle counterParticle = Particle();

    for (auto p : particles)
    {
        double reflectingDistance = std::pow(2, 1.0 / 6) * p->getSigma() / 2;
        counterParticle.setEpsilon(p->getEpsilon()); 
        counterParticle.setSigma(p->getSigma());
        switch (boundary_idx)
        {
        // left boundary, x-coordinate is 0
        case 0:
            if (p->getX()[0] <= reflectingDistance)
            {
                std::array<double, 3> counterX = {(-1) * p->getX()[0], p->getX()[1], p->getX()[2]};
                counterParticle.setX(counterX);
                f(*p, counterParticle);
            }
            break;
        // right boundary, x-coordinate is domain size in x-direction
        case 1:
            if (_domain[0] - p->getX()[0] <= reflectingDistance)
            {
                std::array<double, 3> counterX = {_domain[0] + _domain[0] - p->getX()[0], p->getX()[1], p->getX()[2]};
                counterParticle.setX(counterX);
                f(*p, counterParticle);
            }
            break;
        // lower boundary, y-coordinate is 0
        case 2:
            if (p->getX()[1] <= reflectingDistance)
            {
                std::array<double, 3> counterX = {p->getX()[0], (-1) * p->getX()[1], p->getX()[2]};
                counterParticle.setX(counterX);
                f(*p, counterParticle);
            }
            break;
        // upper boundary, y-coordinate is domain size in y-direction
        case 3:
            if (_domain[1] - p->getX()[1] <= reflectingDistance)
            {
                std::array<double, 3> counterX = {p->getX()[0], _domain[1] + _domain[1] - p->getX()[1], p->getX()[2]};
                counterParticle.setX(counterX);
                f(*p, counterParticle);
            }
            break;
        // front boundary, z-coordinate is 0
        case 4:
            if (p->getX()[2] <= reflectingDistance)
            {
                std::array<double, 3> counterX = {p->getX()[0], p->getX()[1], (-1) * p->getX()[2]};
                counterParticle.setX(counterX);
                f(*p, counterParticle);
            }
            break;
        // back boundary, z-coordinate is domain size in z-direction
        case 5:
            if (_domain[2] - p->getX()[2] <= reflectingDistance)
            {
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
    _haloParticleVector.reserve(newLength);

    // location of vector in memory might change, therefore all particle pointers have to be updated
    for (auto &p : _activeParticleVector)
    {
        p.setInvalid(true);
    }
    for (auto &c : _cellVector)
    {
        c.clearCell();
    }
    updateCells();

    // reserving extra space in each cell with the mean value of particles per cell
    for (auto cell : _cellVector)
    {
        //for number of mean particles per cell consider only domain cells
        cell.reserveMemory(numberOfParticles / ((_numCells[0]-2) * (_numCells[1]-2) * (_numCells[2]-2)));
    }
}

const void LinkedCellParticleContainer::resetParticles()
{
    _activeParticleVector.clear();
    _haloParticleVector.clear();
    for (auto cell : _cellVector)
    {
        cell.clearCell();
    }
}

std::vector<Particle> *LinkedCellParticleContainer::getBoundaryParticles()
{

    auto boundaryParticles = std::make_shared<std::vector<Particle>>();

    for (auto &cell : _cellVector)
    {
        if (cell.getType() == CellType::BoundaryCell)
        {
            for (auto &particle : *cell.getCellParticles())

                boundaryParticles->push_back(*particle);
        }
    }

    return boundaryParticles.get();
}

const void LinkedCellParticleContainer::clearHalo() { 
    _haloParticleVector.clear(); 
    for (auto &cell : _cellVector) {
        if (cell.getType() == CellType::HaloCell) {
            cell.clearCell();
        }
    }
}

const int LinkedCellParticleContainer::size() const { return _activeParticleVector.size(); }

std::vector<ParticleCell> &LinkedCellParticleContainer::getCells() { return _cellVector; }

std::vector<Particle> &LinkedCellParticleContainer::getHaloParticles() { return _haloParticleVector; }

std::vector<Particle> &LinkedCellParticleContainer::getActiveParticles() { return _activeParticleVector; }
