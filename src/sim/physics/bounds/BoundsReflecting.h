//
// Created by alex on 28.11.2022.
//

#pragma once

#include "BoundsFunctorBase.h"

namespace sim::physics::bounds {
    class BoundsReflecting : public BoundsFunctorBase {
    private:
        sim::physics::force::ForceFunctorBase& forceFunctor;

    public:
        ~BoundsReflecting() override = default;

        BoundsReflecting(double st, double et, double dt, double eps, double sig, ParticleContainer &pc, sim::physics::force::ForceFunctorBase& ff)
                : BoundsFunctorBase(st, et, dt, eps, sig, pc), forceFunctor(ff) {}

        /**Reflects particle upon nearing the border.*/
        void operator()() override;
    };
} // sim::physics::bounds
