/*
 * ParticleCell.cpp
 *
 * Created: 02.12.2022
 * Author:  marquardt
 */


#include "ParticleCell.h"
#include "../utils/ArrayUtils.h"
#include <iostream>

ParticleCell::ParticleCell(CellType type, std::array<BoundaryCondition, 6> boundaries) {
    _type = type;
    _boundaries = boundaries;
    _invalidCount = 0;

    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("ParticleCell generated!");
}

ParticleCell::~ParticleCell() {
    _memoryLogger->info("ParticleCell destructed!");
}

const void ParticleCell::insertParticle(Particle *p) {
    //if cell contains invalid particles, replace with new valid one
    if (_invalidCount > 0) {
        for (unsigned long int i = 0; i < _particles.size(); i++) {
            if (_particles[i]->getInvalid() || _particles[i]->getHalo()) {
                _particles[i] = p;
                _invalidCount--;
                break;
            }
        }
    }
    //if there are no invalid particles just add particle to vector
    else {
        _particles.push_back(p);
    } 
}

const void ParticleCell::iterateParticlePairs(std::function<void(Particle &, Particle &)> f, double cutoff) {
    // Since we use a vector we can directly access the particles through indexing
    // Because f_ij = -f_ji, we can save (n^2)/2 iterations by starting the inner loop at i+1
    for (long unsigned int i = 0; i < _particles.size(); i++)
    {
        if (!_particles[i]->getInvalid() && !_particles[i]->getHalo()) {
            for (long unsigned int j = i + 1; j < _particles.size(); j++)
            {   
                if (!_particles[j]->getInvalid() && !_particles[j]->getHalo() && ArrayUtils::L2Norm(_particles[i]->getX() - _particles[j]->getX()) <= cutoff) {
                    f(*_particles[i], *_particles[j]);
                }
            }
        }
    }
}

const void ParticleCell::updateInvalidCounter() {
    _invalidCount = 0;
    for (auto p : _particles) {
        if (p->getInvalid() || p->getHalo()) {
            _invalidCount++;
        }
    }
}

const void ParticleCell::clearCell() { _particles.clear(); }

const void ParticleCell::reserveMemory(int meanParticles) {
    _particles.reserve(_particles.size() - _invalidCount + meanParticles);
}

std::vector<Particle *> &ParticleCell::getCellParticles() { 
    //remove invalid pointers before returning the particles
    if (_invalidCount > 0) {
        _particles.erase(std::remove_if(_particles.begin(), _particles.end(), [](Particle *p) { return p->getInvalid() || p->getHalo(); }), _particles.end());
        _invalidCount = 0;
    }
    return _particles; }

const void ParticleCell::removeInvalid() {
    if (_invalidCount > 0) {
        _particles.erase(std::remove_if(_particles.begin(), _particles.end(), [](Particle *p) { return p->getInvalid() || p->getHalo(); }), _particles.end());
        _invalidCount = 0;
    }
}

const CellType ParticleCell::getType() { return _type; }

const std::array<BoundaryCondition, 6> &ParticleCell::getBoundaries() { return _boundaries; }

const int ParticleCell::size() { return _particles.size() - _invalidCount; }

const std::vector<int> &ParticleCell::getNeighbours() { return _neighbours; }

const void ParticleCell::setNeighbours(std::vector<int> &neighbours) { _neighbours = neighbours; }

