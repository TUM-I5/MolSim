#include <gtest/gtest.h>

#include "../src/LennardJones.h"
#include "../src/utils/Vec3.h"

// test lennard jones force calculations at multiple distances
constexpr double kErrorEpsilon = 1e-4;

const double kSixthRootTwo = std::pow(2, 1.0 / 6.0);
const double kDiagonalSigmaDistance = (kSixthRootTwo / (std::sqrt(2)));

// test struct contains two particleRefs, their expected force, and a set LJ sigma and epsilon
struct LJTestCase {
    Vec3 p_i;
    Vec3 p_j;
    Vec3 expected_force;
    double epsilon = 5.0;
    double sigma = 1.0;
};

// printer for readability on output
static void PrintTo(const LJTestCase &lj, std::ostream *os) {
    *os << "{(" << lj.p_i[0] << "," << lj.p_i[1] << "," << lj.p_i[2] << "), (" << lj.p_j[0] << ","
        << lj.p_j[1] << "," << lj.p_j[2] << ")}";
}

// test struct for Newton's Third Law. Contains only two particleRefs and LJ sigma and epsilon
struct ReversalTestCase {
    Vec3 p_i;
    Vec3 p_j;
    double epsilon = 5.0;
    double sigma = 1.0;
};

// printer for readability on output
static void PrintTo(const ReversalTestCase &rj, std::ostream *os) {
    *os << "{(" << rj.p_i[0] << "," << rj.p_i[1] << "," << rj.p_i[2] << "), (" << rj.p_j[0] << ","
        << rj.p_j[1] << "," << rj.p_j[2] << ")}";
}

// Mock Particle to avoid accessing a particle container
struct MockParticle {
    Vec3 position;

    const Vec3 &x() const { return position; }
};

// fixture for testing against expected force
class LennardJonesTest : public ::testing::TestWithParam<LJTestCase> {};
// alternative fixture for more thorough reversal testing
class ReversalTest : public ::testing::TestWithParam<ReversalTestCase> {};

/**
 * @brief Test computed Lennard Jones force on a particle
 *
 * @param LennardJonesTest test fixture comparing generated force vector against precomputed values
 */
TEST_P(LennardJonesTest, Correct2DForce) {
    const auto &param = GetParam();

    LennardJones lj{.epsilon = param.epsilon, .sigma = param.sigma};

    MockParticle p_i{param.p_i};
    MockParticle p_j{param.p_j};
    const Vec3 resulting_force = lj(p_i, p_j);

    EXPECT_NEAR(resulting_force[0], param.expected_force[0], kErrorEpsilon);
    EXPECT_NEAR(resulting_force[1], param.expected_force[1], kErrorEpsilon);
    // we restrict ourselves to 2 dimensional movement. Any nonzero Z value is erroneous
    EXPECT_NEAR(resulting_force[2], 0.0, kErrorEpsilon);
}

/**
 * @brief Test that force compuatation holds to Newton's third law
 *
 * @param ReversalTest Test struct comparing generated vectors directly instead of
 * against precomputed values
 */
TEST_P(ReversalTest, ParticleSwapForceReversal) {
    const auto &param = GetParam();

    LennardJones lj{.epsilon = param.epsilon, .sigma = param.sigma};

    MockParticle p_i{param.p_i};
    MockParticle p_j{param.p_j};
    const Vec3 resulting_force_a = lj(p_i, p_j);
    const Vec3 resulting_force_b = lj(p_j, p_i);

    EXPECT_NEAR(resulting_force_a[0], -resulting_force_b[0], kErrorEpsilon);
    EXPECT_NEAR(resulting_force_a[1], -resulting_force_b[1], kErrorEpsilon);
    // we restrict ourselves to 2 dimensional movement. Any nonzero Z value is erroneous
    EXPECT_NEAR(resulting_force_a[2], 0.0, kErrorEpsilon);
    EXPECT_NEAR(resulting_force_b[2], 0.0, kErrorEpsilon);
}

// check repulsion
INSTANTIATE_TEST_SUITE_P(RepulsionCases, LennardJonesTest,
                         ::testing::Values(
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {0.5, 0.0, 0.0},
                                 .expected_force = {-1950720.0, 0.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {0.0, 0.5, 0.0},
                                 .expected_force = {0.0, -1950720.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {-0.5, 0.0, 0.0},
                                 .p_j = {0.0, 0.5, 0.0},
                                 .expected_force = {-14400.0, -14400.0, 0.0},
                             }));

// check attraction
INSTANTIATE_TEST_SUITE_P(AttractionCases, LennardJonesTest,
                         ::testing::Values(
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {1.5, 0.0, 0.0},
                                 .expected_force = {5.7901, 0.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {0.0, 1.5, 0.0},
                                 .expected_force = {0.0, 5.7901, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {-1.5, 0.0, 0.0},
                                 .p_j = {0.0, 1.5, 0.0},
                                 .expected_force = {0.4293, 0.4293, 0.0},
                             }));
// check zero force
INSTANTIATE_TEST_SUITE_P(ZeroCases, LennardJonesTest,
                         ::testing::Values(
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {kSixthRootTwo, 0.0, 0.0},
                                 .expected_force = {0.0, 0.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {0.0, kSixthRootTwo, 0.0},
                                 .expected_force = {0.0, 0.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {kDiagonalSigmaDistance, kDiagonalSigmaDistance, 0.0},
                                 .expected_force = {0.0, 0.0, 0.0},
                             }));

// check edge cases - non parameterized for ease
// See that large magnitude repulsions remain finite and do not overflow
TEST(EdgeCases, FiniteForces) {
    LennardJones lj{.epsilon = 5.0, .sigma = 1.0};

    const std::vector<double> dist = {1e-3, 1e-4, 1e-5, 1e-10, 1e-20};

    for (double r : dist) {
        MockParticle p_i{0.0, 0.0, 0.0};
        MockParticle p_j{r, 0.0, 0.0};

        const Vec3 resulting_force = lj(p_i, p_j);

        for (double component : resulting_force) {
            EXPECT_TRUE(std::isfinite(component)) << "Infinite force component at distance " << r;
        }
    }
}

// checked swapped particles reverses force
INSTANTIATE_TEST_SUITE_P(ReversalCases, ReversalTest,
                         ::testing::Values(
                             ReversalTestCase{
                                 .p_i = {-1.5, 0.0, 0.0},
                                 .p_j = {0.0, 1.5, 0.0},
                             },
                             ReversalTestCase{
                                 .p_i = {0.0, 1.5, 0.0},
                                 .p_j = {-1.5, 0, 0.0},
                             },
                             ReversalTestCase{
                                 .p_i = {-0.75, 0.0, 0.0},
                                 .p_j = {0.0, 2.35, 0.0},
                             },
                             ReversalTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {5.0, 0, 0.0},
                             },
                             ReversalTestCase{
                                 .p_i = {-10.0, 0.0, 0.0},
                                 .p_j = {0.0, 10.0, 0.0},
                             }));