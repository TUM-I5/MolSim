#include "LennardJonesForce.h"

#include "utils/ArrayUtils.h"

std::array<double, 3UL> LennardJonesForce::calculateForce(Particle& p, Particle& q) const {
    const auto displacement = q.getX() - p.getX();
    const double dist = ArrayUtils::L2Norm(displacement);

    const auto f_lennard_jones =
        (24 * epsilon / (std::pow(dist, 2))) * (std::pow(sigma / dist, 6) - 2 * std::pow(sigma / dist, 12)) * displacement;

    return f_lennard_jones;
};

LennardJonesForce::operator std::string() const { return "Lennard-Jones"; };