//
// Created by alex on 28.11.2022.
//

#pragma once

#include "BoundsFunctorBase.h"

namespace sim::physics::bounds {
    /**
     * Handles the bound behaviour of the specified side
     * */
    template<sim::physics::bounds::side S>
    class BoundsOutflow : public BoundsFunctorBase<S> {
    public:
        ~BoundsOutflow() override = default;

        BoundsOutflow(double st, double et, double dt, double eps, double sig, ParticleContainer &pc)
                : BoundsFunctorBase<S>(st, et, dt, eps, sig, pc) {}

        /**Removes particles upon crossing the boundary.*/
        void operator()() override {
            std::unordered_set<unsigned long> indices;
            this->particleContainer.template popExternalParticles<S>(indices);
            this->particleContainer.deactivateParticles(indices);
        };
    };
} // sim::physics::bounds

