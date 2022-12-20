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
#include "io/output/Logging.h"

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
    generateForce(type t, double st, double et, double dt, double eps, double sig, ParticleContainer &pc, bool lc, double gGrav) {
        ForceFunctorBase* lcFunctor = nullptr;
        if (lc) {
            if (t != lennardJonesCell && t != lennardJonesGravity) {
                io::output::loggers::general->warn("Linked cell is active but no appropriate force functor was selected.");
                io::output::loggers::general->warn("Ignoring specified force functor, using LennardJonesCell now.");
                t = lennardJonesCell;
            }
            lcFunctor = new FLennardJonesCells(st, et, dt, eps, sig, pc);
        }
        else if (t == lennardJonesCell || t == lennardJonesGravity) {
            t = lennardJones;
        }
        switch (t) {
            case gravity: return new FGravity(st, et ,dt, eps, sig, pc);
            case lennardJones: return new FLennardJones(st, et ,dt, eps, sig, pc);
            case lennardJonesOMP: return new FLennardJonesOMP(st, et ,dt, eps, sig, pc);
            case lennardJonesCell: return lcFunctor;
            case lennardJonesGravity: return new FLennardJonesGravity(st, et, dt, eps, sig, gGrav, pc, lcFunctor);
            default: return new calcF(st, et ,dt, eps, sig, pc);
        }
    }
} // sim::physics::force

