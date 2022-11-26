//
// Created by alex on 26.11.2022.
//

#pragma once

#include "sim/physics/PhysicsFunctorBase.h"

namespace sim::physics::force {

    /**
     * calculate the force for all particles using the Lennard-Jones potential, fast
     * */
    class FLennardJonesOMP : public PhysicsFunctorBase {
    public:
        FLennardJonesOMP(double st,
                         double et,
                         double dt,
                         double eps,
                         double sig,
                         ParticleContainer &pc
        ) : PhysicsFunctorBase(st, et, dt, eps, sig, pc) {}

        void operator()() override;
    };

} // sim::physics::force