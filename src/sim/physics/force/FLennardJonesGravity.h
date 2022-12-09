//
// Created by alex on 06.12.2022.
//

#pragma once

#include "ForceFunctorBase.h"

namespace sim::physics::force {
    /**
     * calculate the force for all particles using the Lennard-Jones potential
     * */
    class FLennardJonesGravity : public ForceFunctorBase {
    private:
        pair_fun_t pairFun;
        ForceFunctorBase* forceDelegate;
        double gGrav;

        void setPairFun();

    public:
        FLennardJonesGravity(double st,
                           double et,
                           double dt,
                           double eps,
                           double sig,
                           double gG,
                           ParticleContainer &pc,
                           ForceFunctorBase* ff
        ) : ForceFunctorBase(st, et, dt, eps, sig, pc), forceDelegate(ff), gGrav(gG) {
            setPairFun();
        }

        void operator()() override;

        void setParticleContainer(ParticleContainer& pc) override;

        pair_fun_t& getForceFunction() override;
    };

} // sim::physics::force
