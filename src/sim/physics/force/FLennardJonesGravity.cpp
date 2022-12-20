//
// Created by alex on 06.12.2022.
//

#include "FLennardJonesGravity.h"

void sim::physics::force::FLennardJonesGravity::setPairFun() {
    pairFun = forceDelegate->getForceFunction();
    fpairFun = forceDelegate->getFastForceFunction();
}

void sim::physics::force::FLennardJonesGravity::operator()() {
    // perform normal force calculation
    forceDelegate->operator()();

    //perform gravity addition
    // we do not care if a particle is still active or not, faster this way
    particleContainer.runOnActiveData([&](std::vector<double> &force,
                                   std::vector<double> &oldForce,
                                   std::vector<double> &x,
                                   std::vector<double> &v,
                                   std::vector<double> &m,
                                   std::vector<int> &type,
                                   unsigned long count,
                                   std::vector<double> &eps,
                                   std::vector<double> &sig,
                                   std::vector<unsigned long> & activeParticles) {
        for (auto index : activeParticles) {
            force[index*3 + 1] += m[index] * gGrav;
        }
    });
}

sim::physics::force::fpair_fun_t &sim::physics::force::FLennardJonesGravity::getFastForceFunction() {
    return fpairFun;
}

void sim::physics::force::FLennardJonesGravity::setParticleContainer(ParticleContainer &pc) {
    particleContainer = pc;
    forceDelegate->setParticleContainer(pc);
    setPairFun();
}

sim::physics::force::pair_fun_t &sim::physics::force::FLennardJonesGravity::getForceFunction() {
    return pairFun;
}
