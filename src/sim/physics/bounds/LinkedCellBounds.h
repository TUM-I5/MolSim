//
// Created by alex on 28.11.2022.
//

#pragma once

#include <string>
#include <unordered_map>
#include "BoundsFunctorBase.h"
#include "sim/physics/force/ForceFunctorBase.h"

namespace sim::physics::bounds {
    enum type {
        outflow,
        reflecting,
        bound_type_count [[maybe_unused]]
    };

    extern std::unordered_map<std::string, type> type_map;

    /**
     * Converts the given string to a known type.
     * */
    type stot(const std::string &);

    /**
     * Generate the correct bounds functor depending on @param t.
     * The other args are passed to the constructor.
     * */
    BoundsFunctorBase*
    generateBounds(type t, sim::physics::force::ForceFunctorBase& ff, double st, double et, double dt, double eps, double sig, ParticleContainer &pc);

} // sim::physics::bounds
