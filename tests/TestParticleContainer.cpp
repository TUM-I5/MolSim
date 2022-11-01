//
// Created by alex on 01.11.2022.
//

#include "doctest.h"
#include "ParticleContainer.h"
#include "Particle.h"

#include <vector>

TEST_CASE("ParticleContainer_Empty") {
    ParticleContainer pc{};
    CHECK((pc.size() == 0));
}

TEST_CASE("ParticleContainer_Count") {
    std::vector<Particle> vec{Particle{0}, Particle{1}, Particle{2}, Particle{3}};
    REQUIRE((vec.size() == 4));

    SUBCASE("Correct Size") {
        ParticleContainer pc{vec};
        CHECK((pc.size() == 4));
    }

    SUBCASE("Correct Data") {
        ParticleContainer pc{vec};
        outer : for (unsigned long i{ 0 }; i < pc.size(); i++) {
            inner : for (unsigned long j { 0 }; j < vec.size(); j++) {
                if (pc.getParticle(i) == vec[j]) goto success;
            } // inner
            fail: FAIL_CHECK("Unknown element in ParticleContainer");
            success: continue; // continue with next particle
        } // outer
    }
}
