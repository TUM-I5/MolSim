//
// Created by alex on 28.11.2022.
//

#pragma once

#include "BoundsFunctorBase.h"
#include "sim/physics/force/ForceFunctorBase.h"

namespace sim::physics::bounds {
    template<sim::physics::bounds::side S>
    class BoundsReflecting : public BoundsFunctorBase<S> {
    private:
        sim::physics::force::ForceFunctorBase& forceFunctor;

    public:
        ~BoundsReflecting() override = default;

        BoundsReflecting(double st, double et, double dt, double eps, double sig, ParticleContainer &pc, sim::physics::force::ForceFunctorBase& ff)
                : BoundsFunctorBase<S>(st, et, dt, eps, sig, pc), forceFunctor(ff) {}

        /**Reflects particle upon nearing the border.*/
        void operator()() override {

        }
    };
} // sim::physics::bounds
