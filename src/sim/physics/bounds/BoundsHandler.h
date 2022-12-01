//
// Created by alex on 30.11.22.
//


#include "BoundsFunctorBase.h"
#include "sim/physics/force/ForceFunctorBase.h"
#include "BoundsOutflow.h"
#include "BoundsReflecting.h"

namespace sim::physics::bounds {
    using bound_t = sim::physics::bounds::type;
    using side_t = sim::physics::bounds::side;

    class BoundsHandler {
    private:
        sim::physics::bounds::BoundsFunctorBase<side_t::left> *handleLeft;
        sim::physics::bounds::BoundsFunctorBase<side_t::right> *handleRight;
        sim::physics::bounds::BoundsFunctorBase<side_t::top> *handleTop;
        sim::physics::bounds::BoundsFunctorBase<side_t::bottom> *handleBottom;
        sim::physics::bounds::BoundsFunctorBase<side_t::front> *handleFront;
        sim::physics::bounds::BoundsFunctorBase<side_t::rear> *handleRear;

    public:
        BoundsHandler() = delete;

        /**
         * @Brief Creates a bounds handler that supports different bounds behaviour for each side.
         * This is defined by the first six parameters.
         * <h3> Will allocate memory on heap </h3>
         * @param ff is the currently use force calculation method.
         * The remaining arguments are simulation properties.
         * */
        BoundsHandler(bound_t let, bound_t rit, bound_t tot, bound_t bot, bound_t frt, bound_t ret,
                      sim::physics::force::ForceFunctorBase &ff, double st, double et, double dt, double eps,
                      double sig, ParticleContainer &pc);

        /**
         * Deallocates used memory.
         * */
        ~BoundsHandler();

        /**
         * Sets the particle container of all bound functors
         * */
        void setParticleContainer(ParticleContainer& pc);

        /**
         * Handles the bound for each side by calling the function operator of each functor.
         * */
        void operator()();
    };

    /**
     * Generate the correct bounds functor depending on @param t.
     * The other args are passed to the constructor.
     * */
    template<sim::physics::bounds::side S>
    static BoundsFunctorBase<S>* generateBound(type t, sim::physics::force::ForceFunctorBase &ff,
                                                        double st, double et, double dt, double eps, double sig,
                                                        ParticleContainer &pc) {
        if (t == bound_t::outflow) return new BoundsOutflow<S>(st, et, dt, eps, sig, pc);
        else if (t == bound_t::reflecting) return new BoundsReflecting<S>(st, et, dt, eps, sig, pc, ff);
        else return new BoundsFunctorBase<S>(st, et, dt, eps, sig, pc);
    }
} // sim::physics::bounds


