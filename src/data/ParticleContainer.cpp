#include "ParticleContainer.h"
#include "Particle.h"
#include <vector>

ParticleContainer::ParticleContainer() = default;

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
