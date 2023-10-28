#include "GravityCalculator.h"

#include "utils/ArrayUtils.h"

void GravityCalculator::calculateForces(ParticleContainer &particles) {
    for (auto &p1 : particles) {
        std::array<double, 3> f{};
        for (auto &p2 : particles) {
            if (p1 == p2) continue;

            double r = ArrayUtils::L2Norm(p1.getX() - p2.getX());
            f = f + (1 / (r * r * r)) * p1.getM() * p2.getM() * (p2.getX() - p1.getX());
        }

        p1.setOldF(p1.getF());
        p1.setF(f);
    }
};
