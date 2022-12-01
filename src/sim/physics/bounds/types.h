//
// Created by alex on 30.11.22.
//

#pragma once

#include <string>

namespace sim::physics::bounds {
    enum side {
        left,
        right,
        top,
        bottom,
        front,
        rear,
        side_count [[maybe_unused]]
    };

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

} //sim::physics::bounds