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
    Thermostat ts(pc, 6, 2, 3, 2);

    //temperature ca. 9
    ASSERT_TRUE(std::abs(ts.computeCurrentTemp() - 9) < 9 * 0.000000001);

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


    //temperature ca. 7
    ASSERT_TRUE(std::abs(ts.computeCurrentTemp() - 7) < 7 * 0.000000001);

    


}