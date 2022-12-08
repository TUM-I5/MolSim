//
// Created by alex on 30.11.22.
//

#pragma once

#include <unordered_map>
#include <string>
#include "ForceFunctorBase.h"

namespace sim::physics::force {
    enum type {
        gravity,
        lennardJones,
        lennardJonesOMP,
        lennardJonesCell,
        lennardJonesGravity,
        types_count [[maybe_unused]]
    };

    extern std::unordered_map<std::string, type> type_map;

    /**
     * Converts the given string to a known type.
     * */
    type stot(const std::string &);

    ForceFunctorBase* generateForce(type t, double st, double et, double dt, double eps, double sig, ParticleContainer &pc);
} //sim::physics::force
