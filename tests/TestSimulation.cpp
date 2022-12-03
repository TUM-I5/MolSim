//
// Created by alex on 06.11.2022.
//
#include <gtest/gtest.h>

static_assert(__cplusplus >= 202002L);

#include <Eigen>
#include "sim/Simulation.h"
#include "data/Particle.h"

/**
 * @brief Test if Force-calculation works as intended
 * 
 */
TEST(Simulation, calculateFLennardJones) {
    // Scenario: Particles along X-Axis -> distance 1 -> force should be 1
    ParticleContainer pc = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0.5, 0, 0}, Eigen::Vector3d{0, 0, 0}, 1.0, 0},
                                  Particle{Eigen::Vector3d{-0.5, 0, 0}, Eigen::Vector3d{0, 0, 0}, 1.0, 0}});
    sim::Simulation simulation{pc, 0, 10, 0.01, 1.0 / 24.0, 1.0, "", "", sim::physics::force::type::lennardJones};

    ASSERT_GE(pc.size(), 2) << "Less than 2 particles in container. Error in ParticleContainer.";
    ASSERT_EQ(pc.getParticle(0).getF(), (Eigen::Vector3d{0, 0, 0}))
                                << "Particle 0 not initialized with force 0";
    ASSERT_EQ(pc.getParticle(1).getF(), (Eigen::Vector3d{0, 0, 0}))
                                << "Particle 1 not initialized with force 0";
    simulation.calcF();

    EXPECT_EQ(pc.getParticle(0).getF(), (Eigen::Vector3d{1, 0, 0}));
    EXPECT_EQ(pc.getParticle(1).getF(), (Eigen::Vector3d{-1, 0, 0}));

}

// Old Test, can be ignored
TEST(Simulation, calculateFGravity) {
    ParticleContainer pc = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0, 0, 0}, Eigen::Vector3d{0, 0, 0}, 1.0, 0},
                                  Particle{Eigen::Vector3d{1, 1, 0}, Eigen::Vector3d{0, 0, 0}, 1.0, 0}});
    sim::Simulation simulation{pc, 0, 10, 0.01, 1.0, 1.0, "", "", sim::physics::force::type::gravity};

    ASSERT_GE(pc.size(), 2);

    simulation.calcF();

    const auto f{pc.getParticle(0).getF()};
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
    ParticleContainer pc = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0, 0, 0}, Eigen::Vector3d{1, 0, 0}, 1.0, 0}});
    sim::Simulation simulation{pc, 0, 10, 0.01, 1.0, 1.0, "", "",
                               sim::physics::force::type::lennardJones,
                               sim::physics::position::type::stoermerVelvet};
    ASSERT_GE(pc.size(), 1);

    const auto x0_old{pc.getParticle(0).getX()};
    simulation.calcX();
    const auto x0_new{pc.getParticle(0).getX()};
    EXPECT_LT(x0_old[0], x0_new[0]);


    pc = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0, 0, 0}, Eigen::Vector3d{0, 1, 0}, 1.0, 0}});
    ASSERT_GE(pc.size(), 1);

    const auto x1_old{pc.getParticle(0).getX()};
    simulation.calcX();
    const auto x1_new{pc.getParticle(0).getX()};
    EXPECT_LT(x1_old[1], x1_new[1]);
}

/**
 * @brief Test if Velocity gets computed as expected
 * 
 */
TEST(Simulation, calculateVStoermerVelvet) {
    spdlog::set_level(static_cast<spdlog::level::level_enum>(0));
    ParticleContainer pc = ParticleContainer(
            std::vector<Particle>{Particle{Eigen::Vector3d{0, 0, 0}, Eigen::Vector3d{0, 0, 0}, 10.0, 0},
                                  Particle{Eigen::Vector3d{1, 0, 0}, Eigen::Vector3d{0, 1, 0}, 0.1, 0}});
    sim::Simulation simulation{pc, 0, 10, 0.01, 1.0, 1.0, "", "",
                               sim::physics::force::type::gravity,
                               sim::physics::position::type::stoermerVelvet,
                               sim::physics::velocity::type::stoermerVelvet};
    constexpr double alpha = 0.01;


    ASSERT_GE(pc.size(), 2);

    const auto v_init{pc.getParticle(1).getV()};
    simulation.calcX();
    simulation.calcF();
    simulation.calcV();
    const auto v_step_1{pc.getParticle(1).getV()};
    simulation.calcX();
    simulation.calcF();
    simulation.calcV();
    const auto v_step_2{pc.getParticle(1).getV()};

    EXPECT_GE(v_init[0], v_step_1[0]);
    const double low = v_init[1] * (1 - alpha);
    const double high = v_init[1] * (1 + alpha);
    EXPECT_GE(v_step_1[1], low);
    EXPECT_LE(v_step_1[1], high);

    EXPECT_GE(v_step_1[0], v_step_2[0]);
    EXPECT_NE(v_step_2[1], v_step_1[1]);
}
