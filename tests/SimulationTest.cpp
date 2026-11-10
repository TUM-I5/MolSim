#include <gtest/gtest.h>

#include "../src/LennardJones.h"
#include "../src/ParticleContainer.h"
#include "../src/Simulation.h"
#include "../src/utils/Vec3.h"

constexpr double kErrorEpsilon = 1e-4;

/**
 *@brief Test whether the system conserves energy
 *
 *Computes summed kinetic and potential energy for each particle in the system against original
 *value. Error scales with delta_t ^ 2
 *
 *Runs a reduced duration simulation querying every ten iterations
 */
TEST(SimulationTest, EnergyConserved) {
    constexpr int kCheckEvery = 10;  // run a check every ten iterations
    constexpr double kDeltaT = 0.0002;

    ParticleContainer particles;
    Vec3 x_one{0.0, 0.0, 0.0}, x_two{0.0, 2.0, 0.0};
    Vec3 v_one{0.0, 0.0, 0.0}, v_two{0.0, -.5, 0.0};
    double m_one = 1, m_two = 1;

    particles.add(x_one, v_one, m_one);
    particles.add(x_two, v_two, m_two);

    LennardJones lj{};
    const double original_energy = particles.CalculateEnergy(lj);

    simulation::Simulation sim{std::move(particles), {0, 100, kDeltaT}};

    const auto post_iter = [&lj, original_energy](const ParticleContainer &p, const int iteration,
                                                  const double /*time*/) {
        if (iteration % kCheckEvery == 0) {
            const double current_energy = p.CalculateEnergy(lj);
            EXPECT_NEAR(original_energy, current_energy, 2000.0 * kDeltaT * kDeltaT);
        }
    };

    sim.Run(post_iter);
}

/**
 * @brief Test whether the system conserves linear momentum
 *
 * Computes net linear momentum of an iteration against an original value
 *
 * Runs a reduced duration simulation querying every ten iterations
 */
TEST(SimulationTest, MomentumConserved) {
    constexpr int kCheckEvery = 10;  // run a check every ten iterations
    constexpr double kDeltaT = 0.0002;

    ParticleContainer particles;
    Vec3 x_one{0.0, 0.0, 0.0}, x_two{0.0, 2.0, 0.0}, x_three{2.0, 0.0, 0.0}, x_four{-1.5, 0.0, 0.0},
        x_five{-3.0, -3.0, 0.0};
    Vec3 v_one{0.0, 0.0, 0.0}, v_two{0.0, -0.5, 0.0}, v_three{0.0, -0.5, 0.0},
        v_four{0.5, 0.5, 0.0}, v_five{0.25, 0.25, 0.0};
    double m_one = 1, m_two = 1, m_three = 3, m_four = 0.5, m_five = 2.0;

    particles.add(x_one, v_one, m_one);
    particles.add(x_two, v_two, m_two);
    particles.add(x_three, v_three, m_three);
    particles.add(x_four, v_four, m_four);
    particles.add(x_five, v_five, m_five);

    Vec3 original_momentum = {0.0, 0.0, 0.0};

    // caculate original net linear momentum
    for (auto p : particles) {
        original_momentum = original_momentum + p.m() * p.v();
    }

    simulation::Simulation sim{std::move(particles), {0, 100, kDeltaT}};

    const auto post_iter = [original_momentum](const ParticleContainer &p, const int iteration,
                                               const double /*time*/) {
        if (iteration % kCheckEvery == 0) {
            // calculate momentum of each iteration
            Vec3 current_momentum = {0.0, 0.0, 0.0};

            for (auto p : p) {
                current_momentum = current_momentum + p.m() * p.v();
            }
            // check each component of the momentum against each other
            EXPECT_NEAR(current_momentum[0], original_momentum[0], kErrorEpsilon);
            EXPECT_NEAR(current_momentum[1], original_momentum[1], kErrorEpsilon);
            // Should always be 0.0 in 2D simulations
            EXPECT_NEAR(current_momentum[2], original_momentum[2], kErrorEpsilon);
        }
    };

    sim.Run(post_iter);
}