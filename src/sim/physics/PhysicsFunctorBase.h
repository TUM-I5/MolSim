//
// Created by alex on 26.11.2022.
//

#pragma once

#include "data/ParticleContainer.h"

namespace sim::physics {
    class PhysicsFunctorBase {
    protected:
        const double start_time;
        const double end_time;
        const double delta_t;
        const double epsilon;
        const double sigma;
        ParticleContainer &particleContainer;

    public:
        PhysicsFunctorBase() = delete;

        PhysicsFunctorBase(double st, double et, double dt, double eps, double sig, ParticleContainer& pc) :
                start_time(st), end_time(et), delta_t(dt), epsilon(eps), sigma(sig), particleContainer(pc) {}

        ~PhysicsFunctorBase() = default;

        virtual void operator()() = 0;

        void setParticleContainer(ParticleContainer& pc) { particleContainer = pc; }
    };

} // sim::physics