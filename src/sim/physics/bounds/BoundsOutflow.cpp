//
// Created by alex on 28.11.2022.
//

#include "BoundsOutflow.h"

#include <unordered_set>

namespace sim::physics::bounds {
    void BoundsOutflow::operator()() {
        std::unordered_set<unsigned long> buffer{};
        particleContainer.getExternalParticles(buffer);
        particleContainer.deactivateParticles(buffer);
    }
}
