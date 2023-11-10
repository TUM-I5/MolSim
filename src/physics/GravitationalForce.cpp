#include "GravitationalForce.h"

#include "utils/ArrayUtils.h"

std::array<double, 3UL> GravitationalForce::calculateForce(Particle& p, Particle& q) {
    double dist = ArrayUtils::L2Norm(p.getX() - q.getX());

    auto f_gravity = (p.getM() * q.getM() / (std::pow(dist, 3))) * (q.getX() - p.getX());

    return f_gravity;
};

std::string GravitationalForce::getName() {
    return "Gravity";
}