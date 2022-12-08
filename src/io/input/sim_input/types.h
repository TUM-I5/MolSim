//
// Created by alex on 07.12.22.
//

#pragma once

#include <unordered_map>
#include <string>

namespace io::input {
    enum type {
        XML,
        FILE,
        BODY,
        type_count [[maybe_unused]]
    };

    extern std::unordered_map<std::string, type> type_map;

    /**
     * Converts the given string to a known type.
     * */
    type stot(const std::string&);
}//io::input