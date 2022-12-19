//
// Created by alex on 26.11.2022.
//

#include "FLennardJonesOMP.h"
#include "FLennardJones.h"

namespace sim::physics::force {
    void FLennardJonesOMP::operator()() {
        particleContainer.runOnData([](std::vector<double> &force,
                                       std::vector<double> &oldForce,
                                       std::vector<double> &x,
                                       std::vector<double> &v,
                                       std::vector<double> &m,
                                       std::vector<int> &type,
                                       unsigned long count,
                                       std::vector<double> &eps,
                                       std::vector<double> &sig) {

            double sigma, epsilon;
            double sigma6;
            double l2NInvSquare, fac0, l2NInvPow6, fac1_sum1, fac1, d0, d1, d2;
            unsigned long indexI;
            unsigned long indexJ;
            unsigned long endIndex = count * (count + 1) / 2;
            double* f = force.data();

#pragma omp parallel default(none) shared(force, oldForce, x, v, m, count, endIndex, f, sig, eps) private(l2NInvSquare, fac0, l2NInvPow6, fac1_sum1, fac1, d0, d1, d2, indexI, indexJ, sigma, sigma6, epsilon )
            {
#pragma omp for reduction(+:f[:count*3])
                for(unsigned long globalIndex = 0; globalIndex < endIndex; globalIndex++){
                    indexI = globalIndex / count;
                    indexJ = globalIndex % count;
                    if (indexI > indexJ) {
                        indexI = count - indexI;
                        indexJ = count - indexJ - 1;
                    }
                    if(indexI == indexJ) continue;
                    sigma = (sig[indexI] + sig[indexJ]) / 2;
                    sigma6 = sigma * sigma * sigma * sigma * sigma * sigma;
                    epsilon = std::sqrt(eps[indexI] * eps[indexJ]); // TODO this can be cached
                    d0 = x[indexI*3 + 0] - x[indexJ*3 + 0];
                    d1 = x[indexI*3 + 1] - x[indexJ*3 + 1];
                    d2 = x[indexI*3 + 2] - x[indexJ*3 + 2];
                    l2NInvSquare = 1 / (d0*d0 + d1*d1 + d2*d2);
                    fac0 = 24 * epsilon * l2NInvSquare;
                    l2NInvPow6 = l2NInvSquare * l2NInvSquare * l2NInvSquare;
                    fac1_sum1 = sigma6 * l2NInvPow6;
                    fac1 = (fac1_sum1) - 2 * (fac1_sum1 * fac1_sum1);

                    f[indexI*3 + 0] += (-1) * fac0 * fac1 * d0;
                    f[indexI*3 + 1] += (-1) * fac0 * fac1 * d1;
                    f[indexI*3 + 2] += (-1) * fac0 * fac1 * d2;
                    f[indexJ*3 + 0] += fac0 * fac1 * d0;
                    f[indexJ*3 + 1] += fac0 * fac1 * d1;
                    f[indexJ*3 + 2] += fac0 * fac1 * d2;
                }
            }
        });
    }

    void FLennardJonesOMP::setParticleContainer(ParticleContainer &pc) {
        PhysicsFunctorBase::setParticleContainer(pc);
        forceDelegate.setParticleContainer(pc);
        setPairFun();
    }

    pair_fun_t &FLennardJonesOMP::getForceFunction() {
        return pairFun;
    }

    void FLennardJonesOMP::setPairFun() {
        pairFun = forceDelegate.getForceFunction();
        fpairFun = forceDelegate.getFastForceFunction();
    }

    fpair_fun_t &FLennardJonesOMP::getFastForceFunction() {
        return fpairFun;
    }
} // sim::physics::force