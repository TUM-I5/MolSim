//
// Created by alex on 28.11.2022.
//

#pragma once

#include "BoundsFunctorBase.h"

namespace sim::physics::bounds {
    class BoundsOutflow : public BoundsFunctorBase {
    public:
        ~BoundsOutflow() override = default;

        BoundsOutflow(double st, double et, double dt, double eps, double sig, ParticleContainer &pc)
                : BoundsFunctorBase(st, et, dt, eps, sig, pc) {}

        /**Removes particles upon crossing the boundary.*/
        void operator()() override;
    };
} // sim::physics::bounds

