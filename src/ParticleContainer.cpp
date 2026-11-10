#include "ParticleContainer.h"

void ParticleContainer::add(const Vec3 &x, const Vec3 &v, const double m, const int type) {
    x_.push_back(x);
    v_.push_back(v);
    f_.push_back(Vec3{});
    old_f_.push_back(Vec3{});
    m_.push_back(m);
    type_.push_back(type);
}

void ParticleContainer::reserve(const std::size_t n) {
    x_.reserve(n);
    v_.reserve(n);
    f_.reserve(n);
    old_f_.reserve(n);
    m_.reserve(n);
    type_.reserve(n);
}
