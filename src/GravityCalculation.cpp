#include "GravityCalculation.h"

#include "utils/ArrayUtils.h"

void GravityCalculation::addForce(Particle& p, Particle& q) {
    double r = ArrayUtils::L2Norm(p.getX() - q.getX());
    p.setF(p.getF() + (1 / (r * r * r)) * p.getM() * q.getM() * (q.getX() - p.getX()));
};
