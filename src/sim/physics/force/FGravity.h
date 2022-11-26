//
// Created by alex on 26.11.2022.
//

#pragma once

#include "sim/physics/PhysicsFunctorBase.h"

namespace sim::physics::force {
    /**
    * calculate the force for all particles by gravitation.
    */
    class FGravity : public PhysicsFunctorBase {
    public:
        FGravity(double st,
                 double et,
                 double dt,
                 double eps,
                 double sig,
                 ParticleContainer &pc
        ) : PhysicsFunctorBase(st, et, dt, eps, sig, pc) {}

        void operator()() override;
    };
} // sim::physics::force