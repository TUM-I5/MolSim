//
// Created by alex on 06.12.2022.
//

#pragma once

#include "BoundsFunctorBase.h"
#include "sim/physics/force/ForceFunctorBase.h"

namespace sim::physics::bounds {
    template<sim::physics::bounds::side S>
    class BoundsPeriodic : public BoundsFunctorBase<S> {
    private:
        sim::physics::force::ForceFunctorBase& forceFunctor;

    public:
        ~BoundsPeriodic() override = default;

        BoundsPeriodic(double st, double et, double dt, double eps, double sig, ParticleContainer &pc, sim::physics::force::ForceFunctorBase& ff)
                : BoundsFunctorBase<S>(st, et, dt, eps, sig, pc), forceFunctor(ff) {}

        /**Reflects particle upon nearing the border.*/
        void operator()() override {
            // TODO implement BoundsPeriodic handler
        }
    };
} // sim::physics::bounds