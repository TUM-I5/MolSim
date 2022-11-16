//
// Created by alex on 30.10.2022.
//

#include "Simulation.h"
#include "data/Particle.h"
#include "data/ParticleContainer.h"

#include "../Eigen/Dense"

#include <omp.h>

namespace sim {
    ParticleContainer particleContainer;
    double start_time = 0;
    double end_time;
    double delta_t;
    double epsilon;
    double sigma;
    std::string outputFolder;
    std::string outputBaseName;

    void calculateFGravity() {
        //set all current forces on all particles to 0
        particleContainer.forAllParticles([&](Particle &p) {
            p.setOldF(p.getF());
            p.setF({0., 0., 0.});
        });

        particleContainer.forAllPairs([](Particle &p1, Particle &p2) {
            double delta_x = p1.getX()[0] - p2.getX()[0];
            double delta_y = p1.getX()[1] - p2.getX()[1];
            double scalar =
                    p1.getM() * p2.getM() * std::pow(1 / std::sqrt(delta_x * delta_x + delta_y * delta_y), 3);
            double F_X = -delta_x * scalar;
            double F_Y = -delta_y * scalar;
            p1.add_to_F({F_X, F_Y, 0.});
            p2.add_to_F({-F_X, -F_Y, 0.});
        });
    }



    void calculateFLennardJones() {
        //set all current forces on all particles to 0
        particleContainer.forAllParticles([&](Particle &p) {
            p.setOldF(p.getF());
            p.setF({0., 0., 0.});
        });

        particleContainer.forAllPairs([](Particle &p1, Particle &p2) {
            Eigen::Vector3d delta{p1.getX() - p2.getX()};
            double l2Norm = delta.norm();
            double l2NInvSquare = 1 / (l2Norm * l2Norm);                        // invert squared norm
            double fac0 = 24 * epsilon * l2NInvSquare;                          // create first factor
            double l2NInvPow6 = l2NInvSquare * l2NInvSquare * l2NInvSquare;     // sixth power of inverted l2 norm
            double sigma6 = sigma * sigma * sigma;
            sigma6 = sigma6 * sigma6;                                           // sixth power of sigma
            double fac1_sum1 = sigma6 * l2NInvPow6;                               // first summand of middle factor
            double fac1 = (fac1_sum1) - 2 * (fac1_sum1 * fac1_sum1);            // create middle factor

            Eigen::Vector3d force{(-1) * fac0 * fac1 * delta};               // bring it all together
            p1.add_to_F(force);
            p2.add_to_F(-force);                                                // reuse fact that F_ij = -F_ji
        });
    }

    void calculateXStoermerVelvet() {
        particleContainer.forAllParticles([&](Particle &p) {
            Eigen::Vector3d x = delta_t * p.getV() + delta_t * delta_t * p.getOldF() / (2 * p.getM());
            p.add_to_X(x);
        });
    }

    void calculateVStoermerVelvet() {
        particleContainer.forAllParticles([&](Particle &p) {
            Eigen::Vector3d v = delta_t * (p.getOldF() + p.getF()) / (2 * p.getM());
            p.add_to_V(v);
        });
    }

    void calculateFLennardJonesFast() {
        particleContainer.runOnData([](std::vector<double> &force,
                                       std::vector<double> &oldForce,
                                       std::vector<double> &x,
                                       std::vector<double> &v,
                                       std::vector<double> &m,
                                       std::vector<int> &type,
                                       unsigned long count) {

            double sigma6 = sigma * sigma * sigma * sigma * sigma * sigma;
            double l2NInvSquare, fac0, l2NInvPow6, fac1_sum1, fac1, d0, d1, d2;
            unsigned long indexI;
            unsigned long indexJ;
            unsigned long endIndex = count * (count + 1) / 2;

            #pragma omp parallel default(none) shared(force, oldForce, x, v, m, count, delta_t, sigma, sigma6, epsilon, endIndex) private(l2NInvSquare, fac0, l2NInvPow6, fac1_sum1, fac1, d0, d1, d2, indexI, indexJ)
            {
                #pragma omp for
                for (unsigned long index = 0; index < count; index++) {
                    oldForce[index*3 + 0] = force[index*3 + 0];
                    oldForce[index*3 + 1] = force[index*3 + 1];
                    oldForce[index*3 + 2] = force[index*3 + 2];
                    force[index*3 + 0] = 0;
                    force[index*3 + 1] = 0;
                    force[index*3 + 2] = 0;
                }

                #pragma omp barrier
                #pragma omp for
                for(unsigned long globalIndex = 0; globalIndex < endIndex; globalIndex++){
                    indexI = globalIndex / count;
                    indexJ = globalIndex % count;
                    if (indexI > indexJ) {
                        indexI = count - indexI;
                        indexJ = count - indexJ - 1;
                    }
                    if(indexI == indexJ) continue;
                    d0 = x[indexI*3 + 0] - x[indexJ*3 + 0];
                    d1 = x[indexI*3 + 1] - x[indexJ*3 + 1];
                    d2 = x[indexI*3 + 2] - x[indexJ*3 + 2];
                    l2NInvSquare = 1 / (d0*d0 + d1*d1 + d2*d2);
                    fac0 = 24 * epsilon * l2NInvSquare;
                    l2NInvPow6 = l2NInvSquare * l2NInvSquare * l2NInvSquare;
                    fac1_sum1 = sigma6 * l2NInvPow6;
                    fac1 = (fac1_sum1) - 2 * (fac1_sum1 * fac1_sum1);

                    #pragma omp critical
                    {
                        force[indexI*3 + 0] += (-1) * fac0 * fac1 * d0;
                        force[indexI*3 + 1] += (-1) * fac0 * fac1 * d1;
                        force[indexI*3 + 2] += (-1) * fac0 * fac1 * d2;
                        force[indexJ*3 + 0] += fac0 * fac1 * d0;
                        force[indexJ*3 + 1] += fac0 * fac1 * d1;
                        force[indexJ*3 + 2] += fac0 * fac1 * d2;
                    }
                }
            }
        });
    }

    void calculateXStoermerVelvetFast() {
        particleContainer.runOnData([](std::vector<double> &force,
                                       std::vector<double> &oldForce,
                                       std::vector<double> &x,
                                       std::vector<double> &v,
                                       std::vector<double> &m,
                                       std::vector<int> &type,
                                       unsigned long count) {
            #pragma omp parallel default(none) shared(force, oldForce, x, v, m, count, delta_t)
            {
                #pragma omp for
                for (unsigned long index = 0; index < count; index++) {
                    x[index*3 + 0] += delta_t * v[index*3 + 0] + delta_t * delta_t * oldForce[index*3 + 0] / (2 * m[index]);
                    x[index*3 + 1] += delta_t * v[index*3 + 1] + delta_t * delta_t * oldForce[index*3 + 1] / (2 * m[index]);
                    x[index*3 + 2] += delta_t * v[index*3 + 2] + delta_t * delta_t * oldForce[index*3 + 2] / (2 * m[index]);
                }
            }
        });
    }

    void calculateVStoermerVelvetFast() {
        particleContainer.runOnData([](std::vector<double> &force,
                                       std::vector<double> &oldForce,
                                       std::vector<double> &x,
                                       std::vector<double> &v,
                                       std::vector<double> &m,
                                       std::vector<int> &type,
                                       unsigned long count) {
            #pragma omp parallel default(none) shared(force, oldForce, v, m, count, delta_t)
            {
                #pragma omp for
                for (unsigned long index = 0; index < count; index++) {
                    v[index*3 + 0] += delta_t * (oldForce[index*3 + 0] + force[index*3 + 0]) / (2 * m[index]);
                    v[index*3 + 1] += delta_t * (oldForce[index*3 + 1] + force[index*3 + 1]) / (2 * m[index]);
                    v[index*3 + 2] += delta_t * (oldForce[index*3 + 2] + force[index*3 + 2]) / (2 * m[index]);
                }
            }
        });
    }
} // sim