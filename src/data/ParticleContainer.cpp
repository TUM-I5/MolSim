#include "ParticleContainer.h"
#include "Particle.h"
#include "io/Logging.h"

#include <vector>

ParticleContainer::ParticleContainer() {
    count = 0;
}

ParticleContainer::ParticleContainer(const std::vector<Particle>& buffer) {
    count = buffer.size();
    force.resize(count*3);
    oldForce.resize(count*3);
    x.resize(count*3);
    v.resize(count*3);
    m.resize(count);
    type.resize(count);

    for (unsigned long index {0}; index < count; index++) {
        auto& f = buffer[index].getF();
        force[index*3 + 0] = f[0];
        force[index*3 + 1] = f[1];
        force[index*3 + 2] = f[2];

        auto& of = buffer[index].getOldF();
        oldForce[index*3 + 0] = of[0];
        oldForce[index*3 + 1] = of[1];
        oldForce[index*3 + 2] = of[2];

        auto& xx = buffer[index].getX();
        x[index*3 + 0] = xx[0];
        x[index*3 + 1] = xx[1];
        x[index*3 + 2] = xx[2];

        auto& vv = buffer[index].getV();
        v[index*3 + 0] = vv[0];
        v[index*3 + 1] = vv[1];
        v[index*3 + 2] = vv[2];

        m[index] = buffer[index].getM();
        type[index] = buffer[index].getType();
    }
}

ParticleContainer::ParticleContainer(const std::vector<Particle>& buffer, std::array<unsigned int, 3> domainSize, unsigned int r_cutoff):
    ParticleContainer::ParticleContainer(buffer)
    {
    //r_cutoff should be a double but how do I check if a is multiple of b if b is r_cutoffa double?
    for(int i{0}; i<domainSize.size(); i++){
        if(domainSize[i]%r_cutoff != 0){
            domainSize[i] = domainSize[i] + r_cutoff - domainSize[i]%r_cutoff;
            loggers::general->debug("Changed the {}-th dimension to {} in order to get a multiple of {}", i, domainSize[i], r_cutoff);
        }
    }
    gridDimensions{domainSize[0]/r_cutoff, (domainSize[1]/r_cutoff), domainSize[2]/r_cutoff};
    //i have no idea why i need helper, it should work without it but the compiler doesn't like it
    std::vector<std::vector<unsigned long>> helper(gridDimensions[0]*gridDimensions[1]*gridDimensions[2],std::vector<unsigned long>{});
    cells = helper;
    this->r_cutoff{(double)r_cutoff}
    }

ParticleContainer::ParticleContainer(const std::vector<Particle>& buffer, 
    std::array<unsigned int, 2> domainSize, unsigned int r_cutoff) : 
    ParticleContainer::ParticleContainer(buffer, {domainSize[0], domainSize[1], r_cutoff}, r_cutoff){};



unsigned long ParticleContainer::size() {
    return count;
}

void ParticleContainer::loadParticle(Particle& p, unsigned long index){
    Eigen::Vector3d f{force[index*3 + 0],
                      force[index*3 + 1],
                      force[index*3 + 2]};
    p.setF(f);
    Eigen::Vector3d of{oldForce[index*3 + 0],
                       oldForce[index*3 + 1],
                       oldForce[index*3 + 2]};
    p.setOldF(of);
    Eigen::Vector3d xx{x[index*3 + 0],
                       x[index*3 + 1],
                       x[index*3 + 2]};
    p.setX(xx);
    Eigen::Vector3d vv{v[index*3 + 0],
                       v[index*3 + 1],
                       v[index*3 + 2]};
    p.setV(vv);
    p.setM(m[index]);
    p.setType(type[index]);
}

void ParticleContainer::storeParticle(Particle &p, unsigned long index) {
    auto& ff = p.getF();
    force[index*3 + 0] = ff[0];
    force[index*3 + 1] = ff[1];
    force[index*3 + 2] = ff[2];

    auto& oof = p.getOldF();
    oldForce[index*3 + 0] = oof[0];
    oldForce[index*3 + 1] = oof[1];
    oldForce[index*3 + 2] = oof[2];

    auto& xxx = p.getX();
    x[index*3 + 0] = xxx[0];
    x[index*3 + 1] = xxx[1];
    x[index*3 + 2] = xxx[2];

    auto& vvv = p.getV();
    v[index*3 + 0] = vvv[0];
    v[index*3 + 1] = vvv[1];
    v[index*3 + 2] = vvv[2];

    m[index] = p.getM();
    type[index] = p.getType();
}

void ParticleContainer::forAllParticles(void (function)(Particle &p)) {
    for (unsigned long index {0}; index < count; index++) {
        Particle p;
        loadParticle(p, index);
        function(p);
        storeParticle(p, index);
    }
}

void ParticleContainer::forAllParticles(std::function<void(Particle&)> function) {
    for (unsigned long index {0}; index < count; index++) {
        Particle p;
        loadParticle(p, index);
        function(p);
        storeParticle(p, index);
    }
}

void ParticleContainer::clear(){
    count = 0;
    force.clear();
    oldForce.clear();
    x.clear();
    v.clear();
    m.clear();
    type.clear();
}

void ParticleContainer::runOnData(
        void (*fun)(std::vector<double> &f, std::vector<double> &of, std::vector<double> &xx, std::vector<double> &vv,
                    std::vector<double> &mm, std::vector<int> &tt, unsigned long cc)) {
    fun(force, oldForce, x, v, m, type, count);
}

void ParticleContainer::forAllPairs(void (*function)(Particle &, Particle &)) {
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

Particle ParticleContainer::getParticle(unsigned long i) {
    Particle p;
    loadParticle(p, i);
    return p;
}

unsigned int ParticleContainer::cellIndexFromCellCoordinates(std::array<unsigned int, 3> coords){
//If we decide to change the order of the cells (e.g. by using some fancy 3d space filling curve)
// this method obviously has to be rewritten

//the current version: 
//cells[0] corresponds to "cells[0][0][0]"
//cells[1] = "cells[1][0][0]"
//cells[domainSize[0]/r_cutoff] = "cells[0][1][0]"
//cells[(domainSize[0]/r_cutoff)/(domainSize[1]/r_cutoff)] = "cells[0][0][1]"
    return (coords[0]+
            coords[1]*gridDimensions[0]+
            coords[2]*gridDimensions[0]+gridDimensions[2]
    );
}   

void ParticleContainer::updateCells(){
    //I am doing an implementation that works first and then i figure out if there is a better way
    //than deciding for every particle in every iteration once again

    //by the way: is there a way to advice a vector not to shrink? i can't find it with like.. 10 mins of googling

    for(auto cell:cells){
        cell.clear();
    }
    for(unsigned int i = 0; i<count; i++){
        //i am intentionally rounding down with casts from double to unsigned int
        std::array<unsigned int, 3>cellCoordinate{(unsigned int) x[3*i]/r_cutoff, (unsigned int) x[3*i +1]/r_cutoff, (unsigned int) x[3*i +2]/r_cutoff};
        this->cells[cellIndexFromCellCoordinates(cellCoordinate)].emplace_back(i);
    }
}

void ParticleContainer::forAllCells(void (*fun)(std::vector<double> &force,
                                std::vector<double> &oldForce,
                                std::vector<double> &x,
                                std::vector<double> &v,
                                std::vector<double> &m,
                                std::vector<int> &type,
                                unsigned long count,
                                std::vector<unsigned long>& cellItems)){
    for(auto cellItems : cells){
        fun(this->force, this->oldForce, this->x, this->v, this->m, this->type, this->count, cellItems);
    }
}


void ParticleContainer::runOnData(void (*fun)(std::vector<double> &force,
                            std::vector<double> &oldForce,
                            std::vector<double> &x,
                            std::vector<double> &v,
                            std::vector<double> &m,
                            std::vector<int> &type,
                            unsigned long count,
                            std::vector<std::vector<unsigned long>>& cells)){
    fun(this->force, this->oldForce, this->x, this->v, this->m, this->type, this->count, this->cells);
}


void ParticleContainer::forAllDistinctCellPairs(void (*fun)(std::vector<double> &force,
                                            std::vector<double> &oldForce,
                                            std::vector<double> &x,
                                            std::vector<double> &v,
                                            std::vector<double> &m,
                                            std::vector<int> &type,
                                            unsigned long count,
                                            std::vector<unsigned long>& cell0Items,
                                            std::vector<unsigned long>& cell1Items)){
    loggers::general->warn("forAllDistinctCellPairs probably wasn't the method you wanted to call you probably wanted to use forAllDistinctCellNeighbours");
    for(unsigned long i = 0; i < cells.size(); i++){
        for(unsigned long j = i+1; j < cells.size(); j++){
            fun(force, oldForce, x, v, m, type, count, cells[i], cells[j]);
        }
    }
}

void forAllDistinctCellNeighbours(void (*fun)(std::vector<double> &force,
                                             std::vector<double> &oldForce,
                                             std::vector<double> &x,
                                             std::vector<double> &v,
                                             std::vector<double> &m,
                                             std::vector<int> &type,
                                             unsigned long count,
                                             std::vector<unsigned long>& cell0Items,
                                             std::vector<unsigned long>& cell1Items)){
    //TODO
    }
