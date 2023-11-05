#include "GravitationalForce.h"

#include "utils/ArrayUtils.h"

std::array<double, 3UL> GravitationalForce::calculateForce(Particle& p, Particle& q) {
    double dist = ArrayUtils::L2Norm(p.getX() - q.getX());

    auto f_gravity = (1 / (dist * dist * dist)) * p.getM() * q.getM() * (q.getX() - p.getX());

    return f_gravity;
};
