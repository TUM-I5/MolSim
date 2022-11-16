//
// Created by alex on 06.11.2022.
//
#include <gtest/gtest.h>

static_assert(__cplusplus >= 202002L);

#include <Eigen>
#include "Simulation.h"
#include "data/Particle.h"

TEST(Simulation, calculateFLennardJonesFast) {
    // TODO
    Eigen::Matrix2d positions;
    positions << 1,2,3,4;
    std::cout<< positions(0) << std::endl;
}

TEST(Simulation, calculateFLennardJones) {
    sim::start_time = 0;
    sim::end_time = 10;
    sim::delta_t = 0.01;
    sim::epsilon = 1.0/24.0;
    sim::sigma = 1;

    // Scenario: Particles along X-Axis -> distance 1 -> force should be 1
    sim::particleContainer = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0.5, 0, 0}, Eigen::Vector3d{0, 0, 0}, 1.0, 0},
                                  Particle{Eigen::Vector3d{-0.5, 0, 0}, Eigen::Vector3d{0, 0, 0}, 1.0, 0}});
    ASSERT_GE(sim::particleContainer.size(), 2) << "Less than 2 particles in container. Error in ParticleContainer.";
    ASSERT_EQ(sim::particleContainer.getParticle(0).getF(), (Eigen::Vector3d{0, 0, 0}))
                                << "Particle 0 not initialized with force 0";
    ASSERT_EQ(sim::particleContainer.getParticle(1).getF(), (Eigen::Vector3d{0, 0, 0}))
                                << "Particle 1 not initialized with force 0";
    sim::calculateFLennardJones();

    EXPECT_EQ(sim::particleContainer.getParticle(0).getF(), (Eigen::Vector3d{1,0,0}));
    EXPECT_EQ(sim::particleContainer.getParticle(1).getF(), (Eigen::Vector3d{-1,0,0}));

}

// Old Test, can be ignored
TEST(Simulation, calculateFGravity) {
    sim::start_time = 0;
    sim::end_time = 10;
    sim::delta_t = 0.01;
    sim::particleContainer = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0, 0, 0}, Eigen::Vector3d{0, 0, 0}, 1.0, 0},
                                  Particle{Eigen::Vector3d{1, 1, 0}, Eigen::Vector3d{0, 0, 0}, 1.0, 0}});

    ASSERT_GE(sim::particleContainer.size(), 2);

    sim::calculateFGravity();

    const auto f{sim::particleContainer.getParticle(0).getF()};
    constexpr double alpha = 0.01;
    const double low = f[0] * (1 - alpha);
    const double high = f[0] * (1 + alpha);
    EXPECT_GE(f[1], low);
    EXPECT_LE(f[1], high);
}
/**
 * Tests if positions are calculated correctly. Velocity is such that outcome is predictable.
 * */
TEST(Simulation, calculateXStoermerVelvet) {
    sim::start_time = 0;
    sim::end_time = 10;
    sim::delta_t = 0.01;
    sim::particleContainer = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0, 0, 0}, Eigen::Vector3d{1, 0, 0}, 1.0, 0}});
    ASSERT_GE(sim::particleContainer.size(), 1);

    const auto x0_old{sim::particleContainer.getParticle(0).getX()};
    sim::calculateXStoermerVelvet();
    const auto x0_new{sim::particleContainer.getParticle(0).getX()};
    EXPECT_LT(x0_old[0], x0_new[0]);


    sim::particleContainer = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0, 0, 0}, Eigen::Vector3d{0, 1, 0}, 1.0, 0}});
    ASSERT_GE(sim::particleContainer.size(), 1);

    const auto x1_old{sim::particleContainer.getParticle(0).getX()};
    sim::calculateXStoermerVelvet();
    const auto x1_new{sim::particleContainer.getParticle(0).getX()};
    EXPECT_LT(x1_old[1], x1_new[1]);
}

TEST(Simulation, calculateVStoermerVelvet) {
    constexpr double alpha = 0.01;
    sim::start_time = 0;
    sim::end_time = 10;
    sim::delta_t = 0.01;

    sim::particleContainer = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0, 0, 0}, Eigen::Vector3d{0, 0, 0}, 10.0, 0},
                                  Particle{Eigen::Vector3d{1, 0, 0}, Eigen::Vector3d{0, 1, 0}, 0.1, 0}});

    ASSERT_GE(sim::particleContainer.size(), 2);

    const auto v_init{sim::particleContainer.getParticle(1).getV()};
    sim::calculateXStoermerVelvet();
    sim::calculateFGravity();
    sim::calculateVStoermerVelvet();
    const auto v_step_1{sim::particleContainer.getParticle(1).getV()};
    sim::calculateXStoermerVelvet();
    sim::calculateFGravity();
    sim::calculateVStoermerVelvet();
    const auto v_step_2{sim::particleContainer.getParticle(1).getV()};

    EXPECT_GE(v_init[0], v_step_1[0]);
    const double low = v_init[1] * (1 - alpha);
    const double high = v_init[1] * (1 + alpha);
    EXPECT_GE(v_step_1[1], low);
    EXPECT_LE(v_step_1[1], high);

    EXPECT_GE(v_step_1[0], v_step_2[0]);
    EXPECT_NE(v_step_2[1], v_step_1[1]);
}
