#include <gtest/gtest.h>

#include "../src/LennardJones.h"
#include "../src/ParticleContainer.h"
#include "../src/Simulation.h"
#include "../src/utils/Vec3.h"

// test
constexpr double kErrorEpsilon = 1e-4;

TEST(SimulationTest, EnergyConserved) {
    constexpr int kCheckEvery = 10;  // run a check every ten iterations

    double original_energy = 0.0;

    ParticleContainer particles;
    Vec3 x_one{0.0, 0.0, 0.0}, x_two{0.0, 1.0, 0.0};
    Vec3 v_one{0.0, 0.0, 0.0}, v_two{-1.0, 0.0, 0.0};
    double m_one = 1, m_two = 1;

    particles.add(x_one, v_one, m_one);
    particles.add(x_two, v_two, m_two);

    LennardJones lj{};
    particles.CalculateEnergy(lj, original_energy);

    simulation::Simulation sim{std::move(particles), {0, 100, .014}};

    const auto post_iter = [&lj, original_energy](const ParticleContainer &p, const int iteration,
                                                  const double /*time*/) {
        if (iteration % kCheckEvery == 0) {
            double current_energy = 0.0;
            p.CalculateEnergy(lj, current_energy);
            EXPECT_NEAR(original_energy, current_energy, kErrorEpsilon);
        }
        std::cout << "Iteration " << iteration << " finished.\n";
    };

    sim.Run(post_iter);
}

// TEST(SimulationTest, ForceConserved) {
//     simulation::Simulation sim{std::move(particles), {0, 100, .014}};
//
//     const auto post_iter = [check_every](const ParticleContainer &p, const int iteration,
//                                          const double /*time*/) {
//         if (iteration % check_every == 0) {
//             // ASSERT...forces sum to zero
//         }
//         std::cout << "Iteration " << iteration << " finished.\n";
//     };
//
//     sim.Run(post_iter);
//}