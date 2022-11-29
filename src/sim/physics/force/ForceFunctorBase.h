//
// Created by alex on 29.11.2022.
//

#pragma once

#include <functional>

#include "sim/physics/PhysicsFunctorBase.h"

namespace sim::physics::force {
    using pair_fun_t = std::function<void(Particle &, Particle &)>;

    class ForceFunctorBase : public PhysicsFunctorBase {
    public:
        ForceFunctorBase(double st, double et, double dt, double eps, double sig, ParticleContainer &pc)
                : PhysicsFunctorBase(st, et, dt, eps, sig, pc) {};

        /**
         * Returns the function that calculates the force between two particles
         * */
        virtual pair_fun_t& getForceFunction() = 0;
    };
} // sim::physics::force


