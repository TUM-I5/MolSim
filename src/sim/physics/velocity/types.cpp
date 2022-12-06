//
// Created by alex on 30.11.22.
//

#include "types.h"
#include "defaults.h"
#include "io/input/cli/CLIArgsParser.h"
#include "VStoermerVelvet.h"
#include "VStoermerVelvetOMP.h"

namespace sim::physics::velocity {
    std::unordered_map<std::string, type> type_map = {{"stoermervelvet",    stoermerVelvet},
                                                      {"stoermervelvetomp", stoermerVelvetOMP}};

    type stot(const std::string &str) {
        auto lowercase = [](std::string &str) {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
        };
        std::string key = str;
        lowercase(key);
        if (!type_map.contains(key)) io::input::exitFormatError("velocity type unknown!");
        else return type_map[key];
    }

    PhysicsFunctorBase *
    generateVelocity(type t, double st, double et, double dt, double eps, double sig, ParticleContainer &pc) {
        switch (t) {
            case stoermerVelvet: return new VStoermerVelvet(st, et ,dt, eps, sig, pc);
            case stoermerVelvetOMP: return new VStoermerVelvetOMP(st, et ,dt, eps, sig, pc);
            default: return new calcV(st, et ,dt, eps, sig, pc);
        }
    }
} // sim::physics::velocity