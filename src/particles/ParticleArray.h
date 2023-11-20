#pragma once

#include <vector>

#include "particles/Particle.h"

class ParticleArray {
   private:
    std::vector<Particle> particles;

   public:
    ParticleArray();

    std::vector<Particle>& getParticles();
};