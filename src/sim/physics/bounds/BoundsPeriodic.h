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
        bool mirrorMinor;
        bool mirrorMajor;

    public:
        ~BoundsPeriodic() override = default;

        BoundsPeriodic(double st, double et, double dt, double eps, double sig, ParticleContainer &pc, bool mMinor, bool mMajor)
                : BoundsFunctorBase<S>(st, et, dt, eps, sig, pc), mirrorMinor(mMinor), mirrorMajor(mMajor) {}

        /**Reflects particle upon nearing the border.*/
        void operator()() override {
            // bounds handler will clear halo cell entries prior to any operator() call of a periodic bound
            // move particles that went beyond the bounds to the other side
            std::unordered_set<unsigned long> indices;
            this->particleContainer.template popExternalParticles<S>(indices);
            this->particleContainer.template moveExternalParticles<S>(indices);
            indices.clear();

            // create halo particles for particles in this side and store in PC (this is tho only place that has knowledge of particles at edges and corners)
            this->particleContainer.template storeBorderParticlesToHalo<S>(this->sigma, mirrorMinor, mirrorMajor);
            // bounds handler will perform force calculation between halo particles and border particles
        }

        static void handleHaloForce(force::ForceFunctorBase& ff, ParticleContainer& pc) {
            pc.template forAllPairsHalo(ff.getForceFunction());
        }
    };
} // sim::physics::bounds