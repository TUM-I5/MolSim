#include "GravitationalForce.h"

#include "utils/ArrayUtils.h"

std::array<double, 3UL> GravitationalForce::calculateForce(Particle& p, Particle& q) const {
    const auto displacement = q.getX() - p.getX();
    const double dist = ArrayUtils::L2Norm(displacement);

    const auto f_gravity = (p.getM() * q.getM() / (std::pow(dist, 3))) * displacement;

    return f_gravity;
};

GravitationalForce::operator std::string() const { return "Gravity"; };