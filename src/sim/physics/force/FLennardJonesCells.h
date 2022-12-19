#pragma once

#include "ForceFunctorBase.h"
#include "FLennardJones.h"

namespace sim::physics::force {
    /**
     * calculate the force for all particles using the Lennard-Jones potential
     * */
    class FLennardJonesCells : public ForceFunctorBase {
    private:
        pair_fun_t pairFun;
        fpair_fun_t fpairFun;
        FLennardJones forceDelegate;

        void setPairFun();

    public:
        FLennardJonesCells(double st,
                      double et,
                      double dt,
                      double eps,
                      double sig,
                      ParticleContainer &pc
        ) : ForceFunctorBase(st, et, dt, eps, sig, pc), forceDelegate(FLennardJones(st, et, dt, eps, sig, pc)) {
            setPairFun();
        }

        void operator()() override;

        void setParticleContainer(ParticleContainer& pc) override;

        pair_fun_t& getForceFunction() override;

        fpair_fun_t& getFastForceFunction() override;
    };

} // sim::physics::force