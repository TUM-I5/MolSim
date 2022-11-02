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
    sim::start_time = 0;
    sim::end_time = 10;
    sim::delta_t = 0.01;

    Particle p1{{0, 0, 0}, {1, 0, 0}, 1.0, 0};
    Particle p2{{1, 0, 0}, {1, 0, 0}, 1.0, 0};
    const auto f1{p1.getF()};
    const auto f2{p2.getF()};

    REQUIRE((f1 == Eigen::Vector3d{0, 0, 0}));
    REQUIRE((f2 == Eigen::Vector3d{0, 0, 0}));

    sim::forceBetw2Particles(p1, p2);
    CHECK((p1.getF() == -p2.getF()));
}

TEST_CASE("calculateF") {
    sim::start_time = 0;
    sim::end_time = 10;
    sim::delta_t = 0.01;
    sim::particleContainer = ParticleContainer(
            std::vector<Particle>{Particle{{0, 0, 0}, {0, 0, 0}, 1.0, 0}, Particle{{1, 1, 0}, {0, 0, 0}, 1.0, 0}});

    REQUIRE((sim::particleContainer.size() >= 2));

    sim::calculateF();

    const auto f {sim::particleContainer.getParticle(0).getF()};
    constexpr double alpha = 0.01;
    const double low = f[0] * (1 - alpha);
    const double high = f[0] * (1 + alpha);
    CHECK((f[1] >= low));
    CHECK((f[1] <= high));
}

TEST_CASE("calculateX") {
    sim::start_time = 0;
    sim::end_time = 10;
    sim::delta_t = 0.01;

    SUBCASE("move x0") {
        sim::particleContainer = ParticleContainer(
                std::vector<Particle>{Particle{{0, 0, 0}, {1, 0, 0}, 1.0, 0}});

        REQUIRE((sim::particleContainer.size() >= 1));

        const auto x_old {sim::particleContainer.getParticle(0).getF()};
        sim::calculateX();
        const auto x_new {sim::particleContainer.getParticle(0).getF()};
        CHECK((x_old[0] < x_new[0]));
    }

    SUBCASE("move x1") {
        sim::particleContainer = ParticleContainer(
                std::vector<Particle>{Particle{{0, 0, 0}, {0, 1, 0}, 1.0, 0}});

        REQUIRE((sim::particleContainer.size() >= 1));

        const auto x_old {sim::particleContainer.getParticle(0).getF()};
        sim::calculateX();
        const auto x_new {sim::particleContainer.getParticle(0).getF()};
        CHECK((x_old[1] < x_new[1]));
    }
}

TEST_CASE("calculateV") {
    constexpr double alpha = 0.01;
    sim::start_time = 0;
    sim::end_time = 10;
    sim::delta_t = 0.01;

    sim::particleContainer = ParticleContainer(
            std::vector<Particle>{Particle{{0, 0, 0}, {0, 0, 0}, 10.0, 0}, Particle{{1, 0, 0}, {0, 0, 0}, 0.1, 0}});

    REQUIRE((sim::particleContainer.size() >= 2));

    const auto v_init {sim::particleContainer.getParticle(0).getV()};

    sim::calculateX();
    sim::calculateF();
    sim::calculateV();

    const auto v_step_1 {sim::particleContainer.getParticle(0).getV()};

    sim::calculateX();
    sim::calculateF();
    sim::calculateV();

    const auto v_step_2 {sim::particleContainer.getParticle(0).getV()};

    CHECK((v_init[0] >= v_step_1[0]));
    const double low = v_init[1] * (1 - alpha);
    const double high = v_init[1] * (1 + alpha);
    CHECK((v_step_1[1] >= low));
    CHECK((v_step_1[1] <= high));

    CHECK((v_step_1[0] >= v_step_2[0]));
    CHECK((v_step_2[1] != v_step_1[1]));
}
