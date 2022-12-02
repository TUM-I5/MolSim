#include "FLennardJonesCells.h"
#include "FLennardJones.h"

namespace sim::physics::force {
    void FLennardJonesCells::operator()() {
        //set all current forces on all particles to 0
        //here we do not care for deactivated particles
        particleContainer.runOnData([=](std::vector<double> &force,
                                                     std::vector<double> &oldForce,
                                                     std::vector<double> &x,
                                                     std::vector<double> &v,
                                                     std::vector<double> &m,
                                                     std::vector<int> &type,
                                                     unsigned long count){
            for (unsigned long index = 0; index < count; index++) {
                oldForce[index*3 + 0] = force[index*3 + 0];
                oldForce[index*3 + 1] = force[index*3 + 1];
                oldForce[index*3 + 2] = force[index*3 + 2];
                force[index*3 + 0] = 0;
                force[index*3 + 1] = 0;
                force[index*3 + 2] = 0;
            }
        });

        particleContainer.forAllCells([=](std::vector<double> &force,
                                         std::vector<double> &oldForce,
                                         std::vector<double> &x,
                                         std::vector<double> &v,
                                         std::vector<double> &m,
                                         std::vector<int> &type,
                                         unsigned long count,
                                         std::vector<unsigned long> &cellItems){
            double sigma6 = sigma * sigma * sigma * sigma * sigma * sigma;
            double l2NInvSquare, fac0, l2NInvPow6, fac1_sum1, fac1, d0, d1, d2;
            double* f = force.data();

            for(unsigned long indexX = 0; indexX < cellItems.size(); indexX++){
                for(unsigned long indexY = indexX + 1; indexY < cellItems.size(); indexY++) {
                    unsigned long indexI = cellItems[indexX];
                    unsigned long indexJ = cellItems[indexY];
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


        //particleContainer.forAllPairsInSameCell(pairFun);
        //particleContainer.forAllPairsInNeighbouringCell(pairFun);
        particleContainer.forAllDistinctCellNeighbours([=](std::vector<double> &force,
                                                           std::vector<double> &oldForce,
                                                           std::vector<double> &x,
                                                           std::vector<double> &v,
                                                           std::vector<double> &m,
                                                           std::vector<int> &type,
                                                           unsigned long count,
                                                           std::vector<unsigned long> &cell0Items,
                                                           std::vector<unsigned long> &cell1Items){
            double sigma6 = sigma * sigma * sigma * sigma * sigma * sigma;
            double l2NInvSquare, fac0, l2NInvPow6, fac1_sum1, fac1, d0, d1, d2;
            double* f = force.data();

            for(unsigned long indexI : cell0Items){
                for(unsigned long indexJ : cell1Items) {
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

    pair_fun_t &FLennardJonesCells::getForceFunction() {
        return pairFun;
    }

    void FLennardJonesCells::setParticleContainer(ParticleContainer &pc) {
        PhysicsFunctorBase::setParticleContainer(pc);
        setPairFun();
    }

    void FLennardJonesCells::setPairFun() {
        pairFun = FLennardJones(start_time, end_time, delta_t, epsilon, sigma, particleContainer).getForceFunction();
    }
} // sim::physics::force
