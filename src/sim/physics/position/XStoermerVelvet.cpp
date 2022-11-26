//
// Created by alex on 26.11.2022.
//

#include "XStoermerVelvet.h"

namespace sim::physics::position {
    void XStoermerVelvet::operator()() {
        particleContainer.forAllParticles([&](Particle &p) {
            Eigen::Vector3d x = delta_t * p.getV() + delta_t * delta_t * p.getOldF() / (2 * p.getM());
            p.add_to_X(x);
        });
    }
} //sim::physics::position
