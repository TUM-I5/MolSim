#include <gtest/gtest.h>

#include "physics/GravitationalForce.h"
#include "utils/ArrayUtils.h"

#define EXPECT_ARRAY_NEAR(a, b, tol)  \
    for (int i = 0; i < 3; i++) {     \
        EXPECT_NEAR(a[i], b[i], tol); \
    };

/*
 * Test if the magnitude of the gravitational force is correct.
 */
TEST(GravityFormula, MagnitudeCorrect) {
    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {0, 0, 0};

    std::array<double, 3> x2 = {1, 1, 1};
    std::array<double, 3> v2 = {0, 0, 0};

    auto p1 = Particle(x1, v1, 1, 0);
    auto p2 = Particle(x2, v2, 1, 1);

    auto gravity = GravitationalForce();

    auto f_gravity = gravity.calculateForce(p1, p2);

    auto expected_mag = std::abs((1 * 1 / (std::pow(ArrayUtils::L2Norm(x1 - x2), 2))));

    EXPECT_NEAR(ArrayUtils::L2Norm(f_gravity), expected_mag, 0.01);
}

/*
 * Test if the direction of the gravitational force is correct.
 */
TEST(GravityFormula, DirectionCorrect) {
    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {0, 0, 0};

    std::array<double, 3> x2 = {1, 1, 1};
    std::array<double, 3> v2 = {0, 0, 0};

    double mass1 = 1;
    double mass2 = 1;

    auto p1 = Particle(x1, v1, mass1, 0);
    auto p2 = Particle(x2, v2, mass2, 1);

    auto gravity = GravitationalForce();

    auto f_gravity = gravity.calculateForce(p1, p2);

    std::vector expected_heading = {std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3};

    auto normalized_f_gravity = (1 / ArrayUtils::L2Norm(f_gravity)) * f_gravity;

    EXPECT_ARRAY_NEAR(normalized_f_gravity, expected_heading, 0.01);
}
