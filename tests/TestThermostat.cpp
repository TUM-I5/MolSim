#include <gtest/gtest.h>

#include "sim/physics/thermostat/Thermostat.h"
#include "data/Particle.h"

TEST(Thermostat, helperFunctionsAndCooling) {
    std::vector<Particle> buffer{};
    std::array<std::array <double, 3>, 3> velocities;
    velocities[0] = {5, 0., 0.};
    velocities[1] = {0.,0.,0.};
    velocities[2] = {0.,5/std::sqrt(2),5/std::sqrt(2)}; //v[2] has length 5 (pointing diagonally)

    Particle p1{std::array<double, 3> {1.,1.,1.}, velocities[0], 4., 0};
    Particle p2{std::array<double, 3> {2.,2.,2.}, velocities[1], 1., 0};
    Particle p3{std::array<double, 3> {2.,1.,1.}, velocities[2], 5., 0};
    buffer.emplace_back(p1);
    buffer.emplace_back(p2);
    buffer.emplace_back(p3);

    ParticleContainer pc(buffer, {4.,4.,4.}, 1.0);
    //Thermostat(ParticleContainer& particleContainer, double T_t, 
    //unsigned int cT = 100, unsigned int dimensions = 2, double dT = std::numeric_limits<double>::infinity()):
    Thermostat ts(pc, 22, 2, 3, 2, 0);

    //temperature ca. 25
    ASSERT_TRUE(std::abs(ts.computeCurrentTemp() - 25) < 25 * 0.000000001)<<"current temp was " << ts.computeCurrentTemp() << " and not approximately 9" << " active Particles: "<< pc.activeSize()<<"\n";

    //vectors unchanged so far
    for(int i{0}; i < 3; i++){
        ASSERT_TRUE(pc.getParticle(i).getV()[0] ==  velocities[i][0] && pc.getParticle(i).getV()[1] ==  velocities[i][1] && pc.getParticle(i).getV()[2] ==  velocities[i][2]);
    }

    ts.notify();

    //vectors unchanged so far
    for(int i{0}; i < 3; i++){
        ASSERT_TRUE(pc.getParticle(i).getV()[0] ==  velocities[i][0] && pc.getParticle(i).getV()[1] ==  velocities[i][1] && pc.getParticle(i).getV()[2] ==  velocities[i][2]);
    }

    ts.notify();


    //temperature ca. 23
    ASSERT_TRUE(std::abs(ts.computeCurrentTemp() - 23) < 23 * 0.000000001) << "current temp was " << ts.computeCurrentTemp() << " instead of 23\n";

    ts.notify();
    ts.notify();

    ASSERT_TRUE(std::abs(ts.computeCurrentTemp() - 22) < 22 * 0.000000001) << "current temp was " << ts.computeCurrentTemp() << " instead of 22\n";

}

TEST(Thermostat, Heating) {
    std::vector<Particle> buffer{};
    std::array<std::array <double, 3>, 3> velocities;
    velocities[0] = {5, 0., 0.};
    velocities[1] = {0.,0.,0.};
    velocities[2] = {0.,5/std::sqrt(2),5/std::sqrt(2)}; //v[2] has length 5 (pointing diagonally)

    Particle p1{std::array<double, 3> {1.,1.,1.}, velocities[0], 4., 0};
    Particle p2{std::array<double, 3> {2.,2.,2.}, velocities[1], 1., 0};
    Particle p3{std::array<double, 3> {2.,1.,1.}, velocities[2], 5., 0};
    buffer.emplace_back(p1);
    buffer.emplace_back(p2);
    buffer.emplace_back(p3);

    ParticleContainer pc(buffer, {4.,4.,4.}, 1.0);
    //Thermostat(ParticleContainer& particleContainer, double T_t,
    //unsigned int cT = 100, unsigned int dimensions = 2, double dT = std::numeric_limits<double>::infinity()):
    Thermostat ts(pc, 30, 2, 3, 3, 0);

    ts.notify();
    //vectors unchanged so far
    for(int i{0}; i < 3; i++){
        ASSERT_TRUE(pc.getParticle(i).getV()[0] ==  velocities[i][0] && pc.getParticle(i).getV()[1] ==  velocities[i][1] && pc.getParticle(i).getV()[2] ==  velocities[i][2]);
    }

    ts.notify();

    //temperature ca. 28
    ASSERT_TRUE(std::abs(ts.computeCurrentTemp() - 28) < 28 * 0.000000001) << "current temp was " << ts.computeCurrentTemp() << " instead of 28\n";

    ts.notify();
    ts.notify();

    //temperature ca. 30
    ASSERT_TRUE(std::abs(ts.computeCurrentTemp() - 30) < 30 * 0.000000001) << "current temp was " << ts.computeCurrentTemp() << " instead of 30\n";
}

TEST(Thermostat, deltaTInf) {
    std::vector<Particle> buffer{};
    std::array<std::array <double, 3>, 3> velocities;
    velocities[0] = {5, 0., 0.};
    velocities[1] = {0.,0.,0.};
    velocities[2] = {0.,5/std::sqrt(2),5/std::sqrt(2)}; //v[2] has length 5 (pointing diagonally)

    Particle p1{std::array<double, 3> {1.,1.,1.}, velocities[0], 4., 0};
    Particle p2{std::array<double, 3> {2.,2.,2.}, velocities[1], 1., 0};
    Particle p3{std::array<double, 3> {2.,1.,1.}, velocities[2], 5., 0};
    buffer.emplace_back(p1);
    buffer.emplace_back(p2);
    buffer.emplace_back(p3);

    ParticleContainer pc(buffer, {4.,4.,4.}, 1.0);
    //Thermostat(ParticleContainer& particleContainer, double T_t,
    //unsigned int cT = 100, unsigned int dimensions = 2, double dT = std::numeric_limits<double>::infinity()):
    Thermostat ts(pc, 30, 2, 3, std::numeric_limits<double>::infinity(), 0);

    ts.notify();
    ts.notify();

    //temperature ca. 30
    ASSERT_TRUE(std::abs(ts.computeCurrentTemp() - 30) < 30 * 0.000000001) << "current temp was " << ts.computeCurrentTemp() << " instead of 30\n";


    //now nothing should happen:
    ts.notify();
    ts.notify();

    //temperature ca. 30
    ASSERT_TRUE(std::abs(ts.computeCurrentTemp() - 30) < 30 * 0.000000001) << "current temp was " << ts.computeCurrentTemp() << " instead of 30\n";
}