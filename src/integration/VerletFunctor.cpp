#include "VerletFunctor.h"

#include "utils/ArrayUtils.h"

void VerletFunctor::step(ParticleContainer& particle_container, std::vector<std::unique_ptr<ForceSource>>& force_sources, double delta_t) {
    for (auto& p : particle_container) {
        // update position
        std::array<double, 3> newX = p.getX() + delta_t * p.getV() + (delta_t * delta_t / (2 * p.getM())) * p.getF();
        p.setX(newX);

        // reset forces
        p.setOldF(p.getF());
        p.setF({0, 0, 0});
    }

    // calculate new forces
    for (auto it1 = particle_container.begin(); it1 != particle_container.end(); ++it1) {
        for (auto it2 = (it1 + 1); it2 != particle_container.end(); ++it2) {
            std::array<double, 3> total_force{0, 0, 0};
            for (auto& force : force_sources) {
                total_force = total_force + force->calculateForce(*it1, *it2);
            }
            it1->setF(it1->getF() + total_force);
            it2->setF(it2->getF() - total_force);
        }
    }

    // update velocity
    for (auto& p : particle_container) {
        std::array<double, 3> newV = p.getV() + (delta_t / (2 * p.getM())) * (p.getF() + p.getOldF());
        p.setV(newV);
    }
}
