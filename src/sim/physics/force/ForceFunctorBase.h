//
// Created by alex on 29.11.2022.
//

#pragma once

#include <functional>

#include "sim/physics/PhysicsFunctorBase.h"

namespace sim::physics::force {
    using pair_fun_t = std::function<void(Particle &, Particle &)>;
    using fpair_fun_t = std::function<void(std::vector<double> &force,
                                           std::vector<double> &x,
                                           std::vector<double> &eps,
                                           std::vector<double> &sig,
                                           std::vector<double> &m,
                                           unsigned long indexI, unsigned long indexJ, bool wbI, bool wbJ)>;

    class ForceFunctorBase : public PhysicsFunctorBase {
    public:
        ForceFunctorBase(double st, double et, double dt, double eps, double sig, ParticleContainer &pc)
                : PhysicsFunctorBase(st, et, dt, eps, sig, pc) {};

        /**
         * Returns the function that calculates the force between two particles
         * */
        virtual pair_fun_t& getForceFunction() = 0;

        /**
         * Returns the fast version of the pairwise force function
         * */
        virtual fpair_fun_t& getFastForceFunction() = 0;

        /**
         * Clears all new Forces and writes them to old force
         * */
        void clearForces() {
            particleContainer.runOnData([](std::vector<double> &force,
                                           std::vector<double> &oldForce,
                                           std::vector<double> &x,
                                           std::vector<double> &v,
                                           std::vector<double> &m,
                                           std::vector<int> &type,
                                           unsigned long count,
                                           std::vector<double> &eps,
                                           std::vector<double> &sig) {
                for (unsigned long index = 0; index < count; index++) {
                    oldForce[index * 3 + 0] = force[index * 3 + 0];
                    oldForce[index * 3 + 1] = force[index * 3 + 1];
                    oldForce[index * 3 + 2] = force[index * 3 + 2];
                    force[index * 3 + 0] = 0;
                    force[index * 3 + 1] = 0;
                    force[index * 3 + 2] = 0;
                }
            });
        }
    };
} // sim::physics::force


