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
        force::ForceFunctorBase& forceFunctor;
        bool mirrorMinor;
        bool mirrorMajor;

    public:
        ~BoundsPeriodic() override = default;

        BoundsPeriodic(double st, double et, double dt, double eps, double sig, ParticleContainer &pc, force::ForceFunctorBase& ff, bool mMinor, bool mMajor)
                : BoundsFunctorBase<S>(st, et, dt, eps, sig, pc), forceFunctor(ff), mirrorMinor(mMinor), mirrorMajor(mMajor) {}

        /**Reflects particle upon nearing the border.
         * Halo construction and force calculation has to be called independently by calling handle Halo.
         * */
        void operator()() final {
            // bounds handler will clear halo cell entries prior to any operator() call of a periodic bound
            // move particles that went beyond the bounds to the other side
            std::unordered_set<unsigned long> indices;
            this->particleContainer.template popExternalParticles<S>(indices);
            this->particleContainer.template moveExternalParticles<S>(indices);
            indices.clear();
        }

        bool isPeriodic() final {return true;}

        void generateHalo() final {
            // create halo particles for particles in this side and store in PC (this is tho only place that has knowledge of particles at edges and corners)
            this->particleContainer.template storeBorderParticlesToHalo<S>(mirrorMinor, mirrorMajor);
        }

        /**Calculates the force for this halo side in periodic bounds.*/
        void calcHaloForce() final {
            this->particleContainer.template forAllPairsHaloSide<S>(forceFunctor.getFastForceFunction());
        }
    };
} // sim::physics::bounds