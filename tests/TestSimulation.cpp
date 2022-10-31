//
// Created by alex on 30.10.2022.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
static_assert(__cplusplus >= 202002L);

#include "Simulation.h"
#include "ParticleContainer.h"

TEST_CASE("calculateF") {
    sim::particleContainer = ParticleContainer();
    REQUIRE((sim::particleContainer.size() == 0));
    SUBCASE("test"){
        CHECK((sim::particleContainer.getParticles().size() == 0));
    }
}

TEST_CASE("calculateX") {

}

TEST_CASE("calculateY") {

}