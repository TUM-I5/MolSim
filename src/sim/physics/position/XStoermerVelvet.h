//
// Created by alex on 26.11.2022.
//

#pragma once

#include "sim/physics/PhysicsFunctorBase.h"

namespace sim::physics::position {

    /**
    * calculate the position for all particles using the Stoermer Velvet method.
    */
    class XStoermerVelvet : public PhysicsFunctorBase {
    public:
        XStoermerVelvet(double st,
                        double et,
                        double dt,
                        double eps,
                        double sig,
                        ParticleContainer &pc
        ) : PhysicsFunctorBase(st, et, dt, eps, sig, pc) {}

        void operator()() override;
    };
} // sim::physics::position