#include <gtest/gtest.h>

#include "../src/LennardJones.h"
#include "../src/utils/Vec3.h"

// test lennard jones force calculations at multiple distances
constexpr double errorEpsilon = 1e-4;

// test struct
struct LJTestCase {
    Vec3 p_i;
    Vec3 p_j;
    Vec3 expectedForce;
    double epsilon = 5.0;
    double sigma = 1.0;
};

// printer for readability on output
void PrintTo(const LJTestCase &lj, std::ostream *os) {
    *os << "{(" << lj.p_i[0] << "," << lj.p_i[1] << "," << lj.p_i[2] << "), (" << lj.p_j[0] << ","
        << lj.p_j[1] << "," << lj.p_j[2] << ")}";
}

// test struct for Newton's Third Law
struct ReversalTestCase {
    Vec3 p_i;
    Vec3 p_j;
    double epsilon = 5.0;
    double sigma = 1.0;
};

// printer for readability on output
void PrintTo(const ReversalTestCase &rj, std::ostream *os) {
    *os << "{(" << rj.p_i[0] << "," << rj.p_i[1] << "," << rj.p_i[2] << "), (" << rj.p_j[0] << ","
        << rj.p_j[1] << "," << rj.p_j[2] << ")}";
}

// Mock Particle to avoid accessing a particle container
struct MockParticle {
    Vec3 position;

    const Vec3 &x() const { return position; }
};

// fixture for close distance testing
class LennardJonesTest : public ::testing::TestWithParam<LJTestCase> {};
// alternative fixture for more thorough reversal testing
class ReversalTest : public ::testing::TestWithParam<ReversalTestCase> {};

TEST_P(LennardJonesTest, Correct2DForce) {
    const auto &param = GetParam();

    LennardJones lj{.epsilon = param.epsilon, .sigma = param.sigma};

    MockParticle p_i{param.p_i};
    MockParticle p_j{param.p_j};
    const Vec3 resultingForce = lj(p_i, p_j);

    EXPECT_NEAR(resultingForce[0], param.expectedForce[0], errorEpsilon);
    EXPECT_NEAR(resultingForce[1], param.expectedForce[1], errorEpsilon);
    // we restrict ourselves to 2 dimensional movement. Any nonzero Z value is erroneous
    EXPECT_NEAR(resultingForce[2], 0.0, errorEpsilon);
}

TEST_P(ReversalTest, ParticleSwapForceReversal) {
    const auto &param = GetParam();

    LennardJones lj{.epsilon = param.epsilon, .sigma = param.sigma};

    MockParticle p_i{param.p_i};
    MockParticle p_j{param.p_j};
    const Vec3 resultingForceA = lj(p_i, p_j);
    const Vec3 resultingForceB = lj(p_j, p_i);

    EXPECT_NEAR(resultingForceA[0], -resultingForceB[0], errorEpsilon);
    EXPECT_NEAR(resultingForceA[1], -resultingForceB[1], errorEpsilon);
    // we restrict ourselves to 2 dimensional movement. Any nonzero Z value is erroneous
    EXPECT_NEAR(resultingForceA[2], 0.0, errorEpsilon);
    EXPECT_NEAR(resultingForceB[2], 0.0, errorEpsilon);
}

INSTANTIATE_TEST_SUITE_P(RepulsionCases, LennardJonesTest,
                         ::testing::Values(
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {0.5, 0.0, 0.0},
                                 .expectedForce = {-1950720.0, 0.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {0.0, 0.5, 0.0},
                                 .expectedForce = {0.0, -1950720.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {-0.5, 0.0, 0.0},
                                 .p_j = {0.0, 0.5, 0.0},
                                 .expectedForce = {-14400.0, -14400.0, 0.0},
                             }));

// check attraction
INSTANTIATE_TEST_SUITE_P(AttractionCases, LennardJonesTest,
                         ::testing::Values(
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {1.5, 0.0, 0.0},
                                 .expectedForce = {5.7901, 0.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {0.0, 1.5, 0.0},
                                 .expectedForce = {0.0, 5.7901, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {-1.5, 0.0, 0.0},
                                 .p_j = {0.0, 1.5, 0.0},
                                 .expectedForce = {0.4293, 0.4293, 0.0},
                             }));
// check zero force
INSTANTIATE_TEST_SUITE_P(ZeroCases, LennardJonesTest,
                         ::testing::Values(
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {1.122462, 0.0, 0.0},
                                 .expectedForce = {0.0, 0.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {0.0, 1.122462, 0.0},
                                 .expectedForce = {0.0, 0.0, 0.0},
                             },
                             LJTestCase{
                                 .p_i = {0.0, 0.0, 0.0},
                                 .p_j = {0.7937, 0.7937, 0.0},
                                 .expectedForce = {0.0, 0.0, 0.0},
                             }));

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
// check edge cases