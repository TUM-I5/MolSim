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

        /**
         * Constructor for utilization by subclasses
         * @param st
         * @param et
         * @param dt
         * @param eps
         * @param sig
         * @param pc
         */
        BoundsFunctorBase(double st, double et, double dt, double eps, double sig, ParticleContainer &pc)
                : PhysicsFunctorBase(st, et, dt, eps, sig, pc) {}

        /**Do nothing, sub classes should still override*/
        void operator()() override {}

        virtual bool isPeriodic() {return false;}

        /**Constructs halo in periodic bound. By default does nothing. Has to be overridden.*/
        virtual void generateHalo() {}

        /**Calculates the force for this halo side in periodic bounds. Has to be overridden.*/
        virtual void calcHaloForce() {}
    };
} //sim::physics::bounds