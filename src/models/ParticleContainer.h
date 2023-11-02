//
// Created by Alp Kaan Aksu on 29.10.23.
//

#pragma once

#include <vector>
#include <functional>

#include "models/Particle.h"

class ParticleContainer {
private:
    std::vector<Particle> particles;
public:
    ParticleContainer();

    void applyToAll(const std::function<void(Particle &)>& function);

    void applyToAllPairs(const std::function<void(Particle &, Particle &)> &function);

    void add(const Particle &particle);

    int size();
};