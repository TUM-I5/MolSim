//
// Created by alex on 28.11.2022.
//

#include "LinkedCellBounds.h"
#include "io/input/cli/CLIArgsParser.h"
#include "BoundsOutflow.h"
#include "BoundsReflecting.h"

namespace sim::physics::bounds {
    std::unordered_map<std::string, type> type_map = {{"outflow", outflow},
                                                      {"reflecting", reflecting}};

    type stot(const std::string &str) {
        auto lowercase = [](std::string &str) {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
        };
        std::string key = str;
        lowercase(key);
        if(!type_map.contains(key)) io::input::exitFormatError("boundary type unknown!");
        else return type_map[key];
    }

    BoundsFunctorBase*
    generateBounds(type type, double st, double et, double dt, double eps, double sig, ParticleContainer &pc) {
        switch(type){
            case outflow: return new BoundsOutflow(st, et, dt, eps, sig, pc);
            case reflecting: return new BoundsReflecting(st, et, dt, eps, sig, pc);
            default: return new BoundsFunctorBase(st, et, dt, eps, sig, pc);
        };
    }

} // sim::physics::bounds