//
// Created by Alp Kaan Aksu on 04.11.23.
//

#pragma once

#include "models/Particle.h"
#include "models/ParticleContainer.h"

namespace Generator {
    void cuboid(ParticleContainer &container, std::array<double, 3> position, std::array<double, 3> size, double meshWidth, std::array<double, 3> velocity, double mass, int typeId);
}
