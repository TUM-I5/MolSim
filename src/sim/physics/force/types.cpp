//
// Created by alex on 30.11.22.
//

#include "defaults.h"
#include "types.h"
#include "io/input/cli/CLIArgsParser.h"
#include "FGravity.h"
#include "FLennardJones.h"
#include "FLennardJonesOMP.h"
#include "FLennardJonesCells.h"
#include "FLennardJonesGravity.h"

namespace sim::physics::force {
    std::unordered_map<std::string, type> type_map = {{"gravity",    gravity},
                                                      {"lennardjones", lennardJones},
                                                      {"lennardjonesomp", lennardJonesOMP},
                                                      {"lennardjonesgravity", lennardJonesGravity},
                                                      {"lennardjonescell", lennardJonesCell}};

    type stot(const std::string &str) {
        auto lowercase = [](std::string &str) {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
        };
        std::string key = str;
        lowercase(key);
        if (!type_map.contains(key)) io::input::exitFormatError("force type unknown!");
        else return type_map[key];
    }

    ForceFunctorBase *
    generateForce(type t, double st, double et, double dt, double eps, double sig, ParticleContainer &pc) {
        switch (t) {
            case gravity: return new FGravity(st, et ,dt, eps, sig, pc);
            case lennardJones: return new FLennardJones(st, et ,dt, eps, sig, pc);
            case lennardJonesOMP: return new FLennardJonesOMP(st, et ,dt, eps, sig, pc);
            case lennardJonesCell: return new FLennardJonesCells(st, et, dt, eps, sig, pc);
            case lennardJonesGravity: return new FLennardJonesGravity(st, et, dt, eps, sig, 0.0, pc, nullptr); // TODO fix this nullptr, this should be selectable at runtime depending on whether cells are used or not
            default: return new calcF(st, et ,dt, eps, sig, pc);                                               // TODO fix above 0.0 this should be gGrav
        }
    }
} // sim::physics::force

