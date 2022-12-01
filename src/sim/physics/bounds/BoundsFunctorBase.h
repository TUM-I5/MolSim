//
// Created by alex on 28.11.2022.
//

#pragma once

#include "sim/physics/PhysicsFunctorBase.h"


namespace sim::physics::bounds {
    template<sim::physics::bounds::side>
    class BoundsFunctorBase : public PhysicsFunctorBase {
    public:
        virtual ~BoundsFunctorBase() = default;

        BoundsFunctorBase(double st, double et, double dt, double eps, double sig, ParticleContainer &pc)
                : PhysicsFunctorBase(st, et, dt, eps, sig, pc) {}

        /**Do nothing, sub classes should still override*/
        void operator()() override {}
    };
} //sim::physics::bounds