//
// Created by alex on 30.10.2022.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
static_assert(__cplusplus >= 202002L);

#include <Eigen>

#include "Simulation.h"
#include "ParticleContainer.h"
#include "Particle.h"

TEST_CASE("forceBetw2Particles") {
    Particle p1 { {0, 0, 0}, {1, 0, 0}, 1.0, 0 };
    Particle p2 { {1, 0, 0}, {1, 0, 0}, 1.0, 0 };
    const auto f1 {p1.getF()};
    const auto f2 {p2.getF()};

    REQUIRE((f1 == Eigen::Vector3d {0, 0, 0}));
    REQUIRE((f2 == Eigen::Vector3d {0, 0, 0}));

    sim::forceBetw2Particles(p1, p2);
    CHECK((p1.getF() == -p2.getF()));
}

TEST_CASE("calculateF") {

}

TEST_CASE("calculateX") {

}

TEST_CASE("calculateY") {

}
