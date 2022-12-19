#include "io/output/Logging.h"
#include "ParticleContainer.h"
#include "Particle.h"

#include <vector>
#include <iostream>
#include <numeric>

#pragma region Contructors

ParticleContainer::ParticleContainer() {
    count = 0;
}

ParticleContainer::ParticleContainer(const std::vector<Particle> &buffer) {
    count = buffer.size();
    force.resize(count * 3);
    oldForce.resize(count * 3);
    x.resize(count * 3);
    v.resize(count * 3);
    m.resize(count);
    type.resize(count);
    eps.resize(count);
    sig.resize(count);

    //define which particles are still part of the simulation
    activeParticles.resize(count);
    std::iota(activeParticles.begin(), activeParticles.end(), 0);

    //load particles
    for (unsigned long index{0}; index < count; index++) {
        auto &f = buffer[index].getF();
        force[index * 3 + 0] = f[0];
        force[index * 3 + 1] = f[1];
        force[index * 3 + 2] = f[2];

        auto &of = buffer[index].getOldF();
        oldForce[index * 3 + 0] = of[0];
        oldForce[index * 3 + 1] = of[1];
        oldForce[index * 3 + 2] = of[2];

        auto &xx = buffer[index].getX();
        x[index * 3 + 0] = xx[0];
        x[index * 3 + 1] = xx[1];
        x[index * 3 + 2] = xx[2];

        auto &vv = buffer[index].getV();
        v[index * 3 + 0] = vv[0];
        v[index * 3 + 1] = vv[1];
        v[index * 3 + 2] = vv[2];

        m[index] = buffer[index].getM();
        type[index] = buffer[index].getType();

        sig[index] = buffer[index].getSigma();
        eps[index] = buffer[index].getEpsilon();
    }
}

ParticleContainer::ParticleContainer(const std::vector<Particle> &buffer, std::array<double, 3> domSize,
                                     double r_cutoff) :
        ParticleContainer::ParticleContainer(buffer) {
    domainSize = domSize;
    x_2_max = domainSize[2];
    x_1_max = domainSize[1];
    x_0_max = domainSize[0];
    x_2_min = 0;
    x_1_min = 0;
    x_0_min = 0;
    //very explicit casts to absolutely make sure, that the rounding is done correctly
    //this implementation uses shorter grids on the side if the numbers are nasty btw
    std::array<double, 3> helperGridDimensions{std::ceil(domSize[0] / r_cutoff), std::ceil(domSize[1] / r_cutoff),
                                               std::ceil(domSize[2] / r_cutoff)};
    gridDimensions = {(unsigned int) helperGridDimensions[0], (unsigned int) helperGridDimensions[1],
                      (unsigned int) helperGridDimensions[2]};

    cells = VectorCoordWrapper(gridDimensions[0]+2, gridDimensions[1]+2, gridDimensions[2]+2);
    this->r_cutoff = (double) r_cutoff;

    updateCells();

    //halo value
    root6_of_2 = std::pow(2, 1/6);
}

ParticleContainer::ParticleContainer(const std::vector<Particle> &buffer, std::array<double, 2> domainSize,
                                     double r_cutoff) :
        ParticleContainer::ParticleContainer(buffer, {domainSize[0], domainSize[1], r_cutoff}, r_cutoff) {};
#pragma endregion

#pragma region Utils

unsigned long ParticleContainer::size() {
    return count;
}

unsigned long ParticleContainer::activeSize() {
    return activeParticles.size();
}

void ParticleContainer::clear() {
    count = 0;
    force.clear();
    oldForce.clear();
    x.clear();
    v.clear();
    m.clear();
    type.clear();
    eps.clear();
    sig.clear();
    activeParticles.clear();
    cells.clear();
}

Particle ParticleContainer::getParticle(unsigned long i) {
    Particle p;
    loadParticle(p, i);
    return p;
}

std::array<unsigned int, 3> ParticleContainer::getGridDimensions() {
    return gridDimensions;
}

void ParticleContainer::loadParticle(Particle &p, unsigned long index, std::vector<double> &force,
                                     std::vector<double> &oldForce, std::vector<double> &x, std::vector<double> &v,
                                     std::vector<double> &m, std::vector<int> &type, std::vector<double>& e,
                                     std::vector<double> &s) {
    Eigen::Vector3d f{force[index * 3 + 0],
                      force[index * 3 + 1],
                      force[index * 3 + 2]};
    p.setF(f);
    Eigen::Vector3d of{oldForce[index * 3 + 0],
                       oldForce[index * 3 + 1],
                       oldForce[index * 3 + 2]};
    p.setOldF(of);
    Eigen::Vector3d xx{x[index * 3 + 0],
                       x[index * 3 + 1],
                       x[index * 3 + 2]};
    p.setX(xx);
    Eigen::Vector3d vv{v[index * 3 + 0],
                       v[index * 3 + 1],
                       v[index * 3 + 2]};
    p.setV(vv);
    p.setM(m[index]);
    p.setType(type[index]);
    p.setEpsilon(e[index]);
    p.setSigma(s[index]);
}

void ParticleContainer::loadParticle(Particle &p, unsigned long index) {
    loadParticle(p, index, force, oldForce, x, v, m, type, eps, sig);
}

void ParticleContainer::storeParticle(Particle &p, unsigned long index, std::vector<double> &force,
                                      std::vector<double> &oldForce, std::vector<double> &x, std::vector<double> &v,
                                      std::vector<double> &m, std::vector<int> &type, std::vector<double>& e,
                                      std::vector<double> &s) {
    auto &ff = p.getF();
    force[index * 3 + 0] = ff[0];
    force[index * 3 + 1] = ff[1];
    force[index * 3 + 2] = ff[2];

    auto &oof = p.getOldF();
    oldForce[index * 3 + 0] = oof[0];
    oldForce[index * 3 + 1] = oof[1];
    oldForce[index * 3 + 2] = oof[2];

    auto &xxx = p.getX();
    x[index * 3 + 0] = xxx[0];
    x[index * 3 + 1] = xxx[1];
    x[index * 3 + 2] = xxx[2];

    auto &vvv = p.getV();
    v[index * 3 + 0] = vvv[0];
    v[index * 3 + 1] = vvv[1];
    v[index * 3 + 2] = vvv[2];

    m[index] = p.getM();
    type[index] = p.getType();
    e[index] = p.getEpsilon();
    s[index] = p.getSigma();

}

void ParticleContainer::storeParticle(Particle &p, unsigned long index) {
    storeParticle(p, index, force, oldForce, x, v, m, type, eps, sig);
}


void ParticleContainer::updateCells() {
    //I am doing an implementation that works first and then i figure out if there is a better way
    //than deciding for every particle in every iteration once again

    //by the way: is there a way to advice a vector not to shrink? i can't find it with like.. 10 mins of googling
    io::output::loggers::general->trace("updateCells called");
    for (auto &cell: cells) {
        cell.clear();
    }
    for (unsigned int i: activeParticles) {
        //i am intentionally rounding down with casts from double to unsigned int
        std::array<unsigned int, 3> cellCoordinate = {0,0,0};
        if(x[3*i+0] > 0) cellCoordinate[0] = (unsigned int) (x[3 * i] / r_cutoff);
        if(x[3*i+1] > 0) cellCoordinate[1] = (unsigned int) (x[3 * i+1] / r_cutoff);
        if(x[3*i+2] > 0) cellCoordinate[2] = (unsigned int) (x[3 * i+2] / r_cutoff);
        this->cells[cellIndexFromCellCoordinates(cellCoordinate)].emplace_back(i);
    }
}

void ParticleContainer::deactivateParticles(std::unordered_set<unsigned long> &indices) {
    activeParticles.erase(std::remove_if(activeParticles.begin(), activeParticles.end(), [&](const auto &item) {
        return indices.contains(item);
    }), activeParticles.end());
}

#pragma endregion

#pragma region Functional

void ParticleContainer::forAllParticles(const std::function<void(Particle &)> &function) {
    for (unsigned long index: activeParticles) {
        Particle p;
        loadParticle(p, index);
        function(p);
        storeParticle(p, index);
    }
}

void ParticleContainer::forAllParticles(void(*function)(Particle &)) {
    for (unsigned long index: activeParticles) {
        Particle p;
        loadParticle(p, index);
        function(p);
        storeParticle(p, index);
    }
}

void ParticleContainer::forAllPairs(const std::function<void(Particle &p1, Particle &p2)> &function) {
    for (u_int32_t i = 0; i < count; i++) {
        for (u_int32_t j = i + 1; j < count; j++) {
            Particle p1;
            loadParticle(p1, i);
            Particle p2;
            loadParticle(p2, j);
            function(p1, p2);
            storeParticle(p1, i);
            storeParticle(p2, j);
        }
    }
}


void ParticleContainer::forAllPairs(void (*function)(Particle &p1, Particle &p2)) {
    for (u_int32_t i = 0; i < count; i++) {
        for (u_int32_t j = i + 1; j < count; j++) {
            Particle p1;
            loadParticle(p1, i);
            Particle p2;
            loadParticle(p2, j);
            function(p1, p2);
            storeParticle(p1, i);
            storeParticle(p2, j);
        }
    }
}

unsigned int ParticleContainer::cellIndexFromCellCoordinatesFast(unsigned int x0, unsigned int x1, unsigned int x2) {
    return (x0 +
            x1 * gridDimensions[0] +
            x2 * gridDimensions[0] * gridDimensions[1]
    );
}

unsigned int ParticleContainer::cellIndexFromCellCoordinates(std::array<unsigned int, 3> coords) {
//If we decide to change the order of the cells (e.g. by using some fancy 3d space filling curve)
// this method obviously has to be rewritten

//the current version:
//cells[0] corresponds to "cells[0][0][0]"
//cells[1] = "cells[1][0][0]"
//cells[domainSize[0]/r_cutoff] = "cells[0][1][0]"
//cells[(domainSize[0]/r_cutoff)/(domainSize[1]/r_cutoff)] = "cells[0][0][1]"
    unsigned int x0 = std::min(coords[0], gridDimensions[0] - 1);
    unsigned int x1 = std::min(coords[1], gridDimensions[1] - 1);
    unsigned int x2 = std::min(coords[2], gridDimensions[2] - 1);

    return (x0 +
            x1 * gridDimensions[0] +
            x2 * gridDimensions[0] * gridDimensions[1]
    );
}

void ParticleContainer::forAllPairsInSameCell(const std::function<void(Particle &p1, Particle &p2)> &function) {
    for (std::vector<unsigned long> &cellItems: cells) {
        for (unsigned long i = 0; i < cellItems.size(); i++) {
            for (unsigned long j = i + 1; j < cellItems.size(); j++) {
                Particle p1, p2;
                loadParticle(p1, cellItems[i]);
                loadParticle(p2, cellItems[j]);
                function(p1, p2);
                storeParticle(p1, cellItems[i]);
                storeParticle(p2, cellItems[j]);
            }
        }
    }
}

[[maybe_unused]] [[maybe_unused]] void ParticleContainer::forAllDistinctCellPairs(
#pragma region param
        void(*fun)(std::vector<double> &force,
                   std::vector<double> &oldForce,
                   std::vector<double> &x,
                   std::vector<double> &v,
                   std::vector<double> &m,
                   std::vector<int> &type,
                   unsigned long count,
                   std::vector<unsigned long> &cell0Items,
                   std::vector<unsigned long> &cell1Items)
#pragma endregion
) {
    io::output::loggers::general->warn(
            "forAllDistinctCellPairs probably wasn't the method you wanted to call you probably wanted to use forAllDistinctCellNeighbours");
    for (unsigned long i = 0; i < cells.size(); i++) {
        for (unsigned long j = i + 1; j < cells.size(); j++) {
            fun(force, oldForce, x, v, m, type, count, cells[i], cells[j]);
        }
    }
}

//this one actually gets used. the one above is tested. they are almost identical except for
void ParticleContainer::forAllPairsInNeighbouringCell(const std::function<void(Particle &p1, Particle &p2)> &function) {

    //Implementation2:
    //basically every code snippet occurs three times right here because every dimension needs to be the "free variable" for every case once
    //but actually this seems more robust than making some fancy "iterate over all possible variable distribution"-thingies

    //Straight lines ----------------------------------------
    //all pairs in x_0 direction:
    for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
        for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0 + 1, x_1, x_2})]) {
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0 + 1, x_1, x_2);
            }
        }
    }
    //all pairs in x_1 direction:
    for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
        for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0, x_1 + 1, x_2})]) {
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0, x_1 + 1, x_2);
            }
        }
    }
    //all pairs in x_2 direction:
    for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
        for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
            for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2 + 1})]) {
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0, x_1, x_2 + 1);
            }
        }
    }
    //End of straight lines ---------------------------------------------------

    //"2d-diagonals"------------------------------------------------
    //diagonals lying in the x_0-x_1 plane
    for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
        //diagonals from bottom left to top right
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0 + 1, x_1 + 1,
                                                                                   x_2})]) { //check with the neighbour that is one to the right and one above you
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0 + 1, x_1 + 1, x_2);
            }
        }
        //diagonals from top left to bottom right
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
            for (unsigned int x_1 = 1; x_1 < gridDimensions[1]; x_1++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0 + 1, x_1 - 1,
                                                                                   x_2})]) { //(check with the neighbour that is one to the right and one below you)
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0 + 1, x_1 - 1, x_2);
            }
        }
    }
    //diagonals lying in the x_0-x_2 plane
    for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
        //diagonals from bottom left to top right
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
            for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0 + 1, x_1, x_2 +
                                                                                                 1})]) { //check with the neighbour that is one to the right and one above you
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0 + 1, x_1, x_2 + 1);
            }
        }
        //diagonals from top left to bottom right
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
            for (unsigned int x_2 = 1; x_2 < gridDimensions[2]; x_2++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0 + 1, x_1, x_2 -
                                                                                                 1})]) { //(check with the neighbour that is one to the right and one below you)
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0 + 1, x_1, x_2 - 1);
            }
        }
    }
    //diagonals lying in the x_1-x_2 plane
    for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
        //diagonals from bottom left to top right
        for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
            for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0, x_1 + 1, x_2 +
                                                                                                 1})]) { //(check with the neighbour that is one to the right and one below you)
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0, x_1 + 1, x_2 + 1);
            }
        }
        //diagonals from top left to bottom right
        for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
            for (unsigned int x_2 = 1; x_2 < gridDimensions[2]; x_2++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0, x_1 + 1, x_2 -
                                                                                                 1})]) { //(check with the neighbour that is one to the right and one below you)
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0, x_1 + 1, x_2 - 1);
            }
        }
    }
    //End of "2d diagonals"-----------------------------------------------

    //Start of "3d diagonals"----------------
    //from bottom front left top back right
    for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
        for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
            for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0 + 1, x_1 + 1, x_2 + 1})]) {
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0 + 1, x_1 + 1, x_2 + 1);
                //std::cout<<"(" << x_0 << ", " << x_1 << ", " << x_2 << ") interacted with (" << x_0+1 << ", " << x_1+1 << ", " << x_2+1 << ")\n";
            }
        }
    }
    //from top front left to bottom back right
    for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
        for (unsigned int x_1 = 1; x_1 < gridDimensions[1]; x_1++) {
            for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0 + 1, x_1 - 1, x_2 + 1})]) {
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0 + 1, x_1 - 1, x_2 + 1);
            }
        }
    }
    //from bottom back left to top front right
    for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
        for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
            for (unsigned int x_2 = 1; x_2 < gridDimensions[2]; x_2++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0 + 1, x_1 + 1, x_2 - 1})]) {
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0 + 1, x_1 + 1, x_2 - 1);
            }
        }
    }
    //from top back left to bottom front right
    for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
        for (unsigned int x_1 = 1; x_1 < gridDimensions[1]; x_1++) {
            for (unsigned int x_2 = 1; x_2 < gridDimensions[2]; x_2++) {

                for (unsigned long index0: cells[cellIndexFromCellCoordinates({x_0, x_1, x_2})]) {
                    for (unsigned long index1: cells[cellIndexFromCellCoordinates({x_0 + 1, x_1 - 1, x_2 - 1})]) {
                        Particle p1, p2;
                        loadParticle(p1, index0);
                        loadParticle(p2, index1);
                        function(p1, p2);
                        storeParticle(p1, index0);
                        storeParticle(p2, index1);
                    }
                }
                io::output::loggers::general->trace("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                                    x_0 + 1, x_1 - 1, x_2 - 1);
            }
        }
    }
}

void ParticleContainer::clearStoreForce() {
    for(unsigned long i {0}; i < count; i++) {
        oldForce[3*i + 0] = force[3*i + 0];
        oldForce[3*i + 1] = force[3*i + 1];
        oldForce[3*i + 2] = force[3*i + 2];
        force[3*i + 0] = 0;
        force[3*i + 1] = 0;
        force[3*i + 2] = 0;
    }
}

#pragma endregion

