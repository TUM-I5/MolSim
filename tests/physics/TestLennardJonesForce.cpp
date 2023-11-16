#include <gtest/gtest.h>

#include "physics/LennardJonesForce.h"
#include "utils/ArrayUtils.h"

#define EXPECT_ARRAY_NEAR(a, b, tol)  \
    for (int i = 0; i < 3; i++) {     \
        EXPECT_NEAR(a[i], b[i], tol); \
    };

/*
 * Test if the magnitude of the lennard jones force is correct.
 */
TEST(LennardJonesFormula, MagnitudeCorrect) {
    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {0, 0, 0};

    std::array<double, 3> x2 = {1, 0, 0};
    std::array<double, 3> v2 = {0, 0, 0};

    auto p1 = Particle(x1, v1, 1, 0);
    auto p2 = Particle(x2, v2, 1, 1);

    auto lennardjones = LennardJonesForce();

    auto f_lennardjones = lennardjones.calculateForce(p1, p2);

    auto expected_mag = std::abs((24 * lennardjones.epsilon) * (std::pow(lennardjones.sigma, 6) - 2 * std::pow(lennardjones.sigma, 12)));

    EXPECT_NEAR(ArrayUtils::L2Norm(f_lennardjones), expected_mag, 0.01);
}

/*
 * Test if the direction of the lennard jones force is correct.
 */
TEST(LennardJonesFormula, DirectionCorrect) {
    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {0, 0, 0};

    std::array<double, 3> x2 = {1, 1, 1};
    std::array<double, 3> v2 = {0, 0, 0};

    double mass1 = 1;
    double mass2 = 1;

    auto p1 = Particle(x1, v1, mass1, 0);
    auto p2 = Particle(x2, v2, mass2, 1);

    auto lennardjones = LennardJonesForce();

    auto f_lennardjones = lennardjones.calculateForce(p1, p2);

    std::vector expected_heading = {std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3};

    auto normalized_lennardjones = (1 / ArrayUtils::L2Norm(f_lennardjones)) * f_lennardjones;

    EXPECT_ARRAY_NEAR(normalized_lennardjones, expected_heading, 0.01);
}
