//
// Created by alex on 26.11.2022.
//

#include "FLennardJones.h"

namespace sim::physics::force {
    void FLennardJones::operator()() {
        //set all current forces on all particles to 0
        particleContainer.forAllParticles([&](Particle &p) {
            p.setOldF(p.getF());
            p.setF({0., 0., 0.});
        });

        particleContainer.forAllPairs(pairFun);
    }

    pair_fun_t &FLennardJones::getForceFunction() {
        return pairFun;
    }

    void FLennardJones::setParticleContainer(ParticleContainer &pc) {
        PhysicsFunctorBase::setParticleContainer(pc);
        setPairFun();
    }

    void FLennardJones::setPairFun() {
        pairFun = [this](Particle &p1, Particle &p2) {
            Eigen::Vector3d delta{p1.getX() - p2.getX()};
            double l2Norm = delta.norm();
            double l2NInvSquare = 1 / (l2Norm * l2Norm);                        // invert squared norm
            double fac0 = 24 * epsilon * l2NInvSquare;                          // create first factor
            double l2NInvPow6 = l2NInvSquare * l2NInvSquare * l2NInvSquare;     // sixth power of inverted l2 norm
            double sigma6 = sigma * sigma * sigma;
            sigma6 = sigma6 * sigma6;                                           // sixth power of sigma
            double fac1_sum1 = sigma6 * l2NInvPow6;                             // first summand of middle factor
            double fac1 = (fac1_sum1) - 2 * (fac1_sum1 * fac1_sum1);            // create middle factor

            Eigen::Vector3d force{(-1) * fac0 * fac1 * delta};                  // bring it all together
            p1.add_to_F(force);
            p2.add_to_F(-force);                                                // reuse fact that F_ij = -F_ji
        };
    }
} // sim::physics::force
