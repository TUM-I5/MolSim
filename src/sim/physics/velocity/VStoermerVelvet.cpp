//
// Created by alex on 26.11.2022.
//

#include "VStoermerVelvet.h"

namespace sim::physics::velocity {
    void VStoermerVelvet::operator()() {
        particleContainer.forAllParticles([&](Particle &p) {
            Eigen::Vector3d v = delta_t * (p.getOldF() + p.getF()) / (2 * p.getM());
            p.add_to_V(v);
        });
    }
} //sim::physics::velocity