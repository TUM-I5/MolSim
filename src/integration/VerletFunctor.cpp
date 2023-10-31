#include "VerletFunctor.h"

#include "utils/ArrayUtils.h"

void VerletFunctor::step(ParticleContainer& particle_container, ForceSource& totalForce, double delta_t) {
    for (auto& p : particle_container) {
        // update position
        std::array<double, 3> newX = p.getX() + delta_t * p.getV() + (delta_t * delta_t / (2 * p.getM())) * p.getF();
        p.setX(newX);

        // reset forces
        p.setOldF(p.getF());
        p.setF({0, 0, 0});
    }

    // calculate new forces
    for (Particle& p : particle_container) {
        for (Particle& q : particle_container) {
            if (&p == &q) {
                continue;
            }

            p.setF(p.getF() + totalForce.calculateForce(p, q));
        }
    }

    // update velocity
    for (auto& p : particle_container) {
        std::array<double, 3> newV = p.getV() + (delta_t / (2 * p.getM())) * (p.getF() + p.getOldF());
        p.setV(newV);
    }
}
