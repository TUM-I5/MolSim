//
// Created by alex on 26.11.2022.
//

#include "FLennardJones.h"

namespace sim::physics::force {
    /**
     * @brief Calculates the using the All-Pairs algorithm. As displayed in the plot the Linked-Cell algorithm is much more performant in comparison
     * \image html plot.png width=800px
     * \image latex plot.eps "Runtime comparison of All-Pairs algorithm with Linked-Cell algorithm" width=5cm
     */
    void FLennardJones::operator()() {
        particleContainer.runOnData([](std::vector<double> &force,
                                       std::vector<double> &oldForce,
                                       std::vector<double> &x,
                                       std::vector<double> &v,
                                       std::vector<double> &m,
                                       std::vector<int> &type,
                                       unsigned long count,
                                       std::vector<double> &eps,
                                       std::vector<double> &sig){
            for (unsigned long index = 0; index < count; index++) {
                oldForce[index*3 + 0] = force[index*3 + 0];
                oldForce[index*3 + 1] = force[index*3 + 1];
                oldForce[index*3 + 2] = force[index*3 + 2];
                force[index*3 + 0] = 0;
                force[index*3 + 1] = 0;
                force[index*3 + 2] = 0;
            }



            double sigma, sigma6, epsilon;
            double l2NInvSquare, fac0, l2NInvPow6, fac1_sum1, fac1, d0, d1, d2;
            double* f = force.data();

            for(unsigned long indexI = 0; indexI < count; indexI++){
                for(unsigned long indexJ = indexI + 1; indexJ < count; indexJ++) {
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


    pair_fun_t &FLennardJones::getForceFunction() {
        return pairFun;
    }

    void FLennardJones::setParticleContainer(ParticleContainer &pc) {
        PhysicsFunctorBase::setParticleContainer(pc);
        setPairFun();
    }

    void FLennardJones::setPairFun() {
        pairFun = [](Particle &p1, Particle &p2) {
            Eigen::Vector3d delta{p1.getX() - p2.getX()};
            double l2Norm = delta.norm();
            double l2NInvSquare = 1 / (l2Norm * l2Norm);                        // invert squared norm
            double epsilon = std::sqrt(p1.getEpsilon()*p2.getEpsilon());
            double fac0 = 24 * epsilon * l2NInvSquare;                          // create first factor
            double l2NInvPow6 = l2NInvSquare * l2NInvSquare * l2NInvSquare;     // sixth power of inverted l2 norm
            double sigma = (p1.getSigma()+p2.getSigma())/2;
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
