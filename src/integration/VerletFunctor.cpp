#include "VerletFunctor.h"

#include "utils/ArrayUtils.h"

void VerletFunctor::step(std::unique_ptr<ParticleContainer>& particle_container,
                         const std::vector<std::unique_ptr<ForceSource>>& force_sources, double delta_t) const {
    for (auto& p : *particle_container) {
        // update position
        const std::array<double, 3> newX = p.getX() + delta_t * p.getV() + (delta_t * delta_t / (2 * p.getM())) * p.getF();
        p.setX(newX);

        // reset forces
        p.setOldF(p.getF());
        p.setF({0, 0, 0});
    }

    // calculate new forces
    particle_container->applyPairwiseForces(force_sources);

    // update velocity
    for (auto& p : *particle_container) {
        const std::array<double, 3> newV = p.getV() + (delta_t / (2 * p.getM())) * (p.getF() + p.getOldF());
        p.setV(newV);
    }
}
