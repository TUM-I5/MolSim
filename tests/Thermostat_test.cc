#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "../src/simulation/Thermostat.h"
#include "../src/model/Particle.h"
#include "../src/model/LinkedCellParticleContainer.h"


// Basic test where temp is instantly set to zero
TEST(Thermostat, NoDeltaToZero) 
{
    BoundaryCondition o = BoundaryCondition::Outflow;
    BoundaryCondition r = BoundaryCondition::Reflecting;
    std::array<double, 3> domain = {80.0, 80.0, 1.0};
    std::array<BoundaryCondition, 6> bound = {r, r, r, r, o, o};
    double reflectingDistance = 0.5;
    double cutoff = 3;
    auto pc = std::make_shared<LinkedCellParticleContainer>(reflectingDistance, cutoff, domain, bound);

    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> v = {100.0, 100.0, 100.0};
    double m = 1;
    double sigma = 1;
    double epsilon = 5;
    
    pc->addParticle(x, v, m, epsilon, sigma);

    Thermostat t = Thermostat(pc, 100.0);
    t.setTargetTemperature(0.0);

    t.apply();

    for (auto &p : pc->getActiveParticles()) {
        EXPECT_TRUE(p.getV()[0] == 0 && p.getV()[1] == 0 && p.getV()[2] == 0);
    }
}

// Test behavior if current temp is already at zero
TEST(Thermostat, AlreadyZero)
{
    BoundaryCondition o = BoundaryCondition::Outflow;
    BoundaryCondition r = BoundaryCondition::Reflecting;
    std::array<double, 3> domain = {80.0, 80.0, 1.0};
    std::array<BoundaryCondition, 6> bound = {r, r, r, r, o, o};
    double reflectingDistance = 0.5;
    double cutoff = 3;
    auto pc = std::make_shared<LinkedCellParticleContainer>(reflectingDistance, cutoff, domain, bound);

    std::array<double, 3> x1 = {1.0, 1.0, 0.0};
    std::array<double, 3> x2 = {79.0, 79.0, 79.0};
    std::array<double, 3> v = {0.0, 0.0, 0.0};
    double m = 1;
    double sigma = 1;
    double epsilon = 5;
    
    pc->addParticle(x1, v, m, epsilon, sigma);
    pc->addParticle(x2, v, m, epsilon, sigma);

    Thermostat t = Thermostat(pc, 0.0);
    t.setTargetTemperature(0.0);

    t.apply();

    for (auto &p : pc->getActiveParticles()) {
        EXPECT_TRUE(p.getV()[0] == 0 && p.getV()[1] == 0 && p.getV()[2] == 0);
    }
}

// Heating
TEST(Thermostat, Heating) 
{
    BoundaryCondition o = BoundaryCondition::Outflow;
    BoundaryCondition r = BoundaryCondition::Reflecting;
    std::array<double, 3> domain = {80.0, 80.0, 1.0};
    std::array<BoundaryCondition, 6> bound = {r, r, r, r, o, o};
    double reflectingDistance = 0.5;
    double cutoff = 3;
    auto pc = std::make_shared<LinkedCellParticleContainer>(reflectingDistance, cutoff, domain, bound);

    std::array<double, 3> x1 = {1.0, 1.0, 0.0};
    std::array<double, 3> x2 = {79.0, 79.0, 79.0};
    std::array<double, 3> v = {1.0, 1.0, 1.0};
    double m = 1;
    double sigma = 1;
    double epsilon = 5;
    
    pc->addParticle(x1, v, m, epsilon, sigma);
    pc->addParticle(x2, v, m, epsilon, sigma);

    Thermostat t = Thermostat(pc, 1.0);
    t.setTargetTemperature(100.0);
    t.setTemperatureDelta(0.5);

    for (int i = 0; i < 1000; i++) {
        float oldTemp = t.calculateCurrentTemperature();
        t.apply();
        float newTemp = t.calculateCurrentTemperature();
        EXPECT_TRUE(oldTemp <= newTemp);
    }
}

// Cooling
TEST(Thermostat, Cooling) 
{
    BoundaryCondition o = BoundaryCondition::Outflow;
    BoundaryCondition r = BoundaryCondition::Reflecting;
    std::array<double, 3> domain = {80.0, 80.0, 1.0};
    std::array<BoundaryCondition, 6> bound = {r, r, r, r, o, o};
    double reflectingDistance = 0.5;
    double cutoff = 3;
    auto pc = std::make_shared<LinkedCellParticleContainer>(reflectingDistance, cutoff, domain, bound);

    std::array<double, 3> x1 = {1.0, 1.0, 0.0};
    std::array<double, 3> x2 = {79.0, 79.0, 79.0};
    std::array<double, 3> v = {1000.0, 1000.0, 1000.0};
    double m = 1;
    double sigma = 1;
    double epsilon = 5;
    
    pc->addParticle(x1, v, m, epsilon, sigma);
    pc->addParticle(x2, v, m, epsilon, sigma);

    Thermostat t = Thermostat(pc, 100.0);
    t.setTargetTemperature(0.0);
    t.setTemperatureDelta(0.5);

    for (int i = 0; i < 1000; i++) {
        float oldTemp = t.calculateCurrentTemperature();
        t.apply();
        float newTemp = t.calculateCurrentTemperature();
        EXPECT_TRUE(oldTemp >= newTemp);
    }
}

// Holding Temperature
TEST(Thermostat, HoldingTemperature)
{
    BoundaryCondition o = BoundaryCondition::Outflow;
    BoundaryCondition r = BoundaryCondition::Reflecting;
    std::array<double, 3> domain = {80.0, 80.0, 1.0};
    std::array<BoundaryCondition, 6> bound = {r, r, r, r, o, o};
    double reflectingDistance = 0.5;
    double cutoff = 3;
    auto pc = std::make_shared<LinkedCellParticleContainer>(reflectingDistance, cutoff, domain, bound);

    std::array<double, 3> x1 = {1.0, 1.0, 0.0};
    std::array<double, 3> x2 = {79.0, 79.0, 79.0};
    std::array<double, 3> v = {0.0, 0.0, 0.0};
    double m = 1;
    double sigma = 1;
    double epsilon = 5;
    
    pc->addParticle(x1, v, m, epsilon, sigma);
    pc->addParticle(x2, v, m, epsilon, sigma);

    Thermostat t = Thermostat(pc, 100.0);
    t.setTargetTemperature(100.0);
    t.setTemperatureDelta(0.5);
    t.initializeBrownianMotion();

    for (int i = 0; i < 1000; i++) {
        float oldTemp = t.calculateCurrentTemperature();
        t.apply();
        float newTemp = t.calculateCurrentTemperature();
        EXPECT_TRUE(std::abs(newTemp - oldTemp) < 1.0);
    }
}