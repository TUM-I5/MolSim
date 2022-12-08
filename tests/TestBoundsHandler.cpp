#include <gtest/gtest.h>

#include "sim/physics/bounds/BoundsHandler.h"
#include "sim/physics/force/FLennardJonesCells.h"
#include "data/Particle.h"

#include <numeric>
#include <vector>
#include <cmath>

/**
 * test if particles outside of domain actually get deleted
 */
TEST(BoundsHandler, outflow){
    std::vector<Particle> particles{};
    std::array<double, 3> pos{4,4,4};
    Particle p1{pos,{0,0,0}, 1, 0};
    pos = {3.2, 1.,1.};
    Particle p2{pos,{0,0,0}, 1, 0};
    particles.emplace_back(p1);
    particles.emplace_back(p2);
    ParticleContainer pc{particles, {3.0,3.0,3.0}, 1.0};
    auto func = sim::physics::force::FLennardJonesCells(0, 1, 0.1, 1, 1, pc);
    sim::physics::bounds::BoundsHandler bh{sim::physics::bounds::outflow, sim::physics::bounds::outflow,
                                           sim::physics::bounds::outflow,
                                           sim::physics::bounds::outflow, sim::physics::bounds::outflow,
                                           sim::physics::bounds::outflow,
                                           func,
                                           0, 1, 0.1, 1, 1,
                                           pc
    };
    ASSERT_TRUE(pc.activeSize() == 2);
    bh.operator()();
    ASSERT_TRUE(pc.activeSize() == 0);
}

/**
 * @brief Test if reflection triggers at the right distance and acts with the right amount of force
 * 
 */
TEST(BoundsHandler, reflecting){
    double newSigma{1/(std::pow(2,1/6))};
    std::vector<Particle> particles{};
    std::array<double, 3> pos{2.01,1.01,1.01};
    Particle p1{pos,{0,0,0}, 1, 0};
    Particle p1Copy{p1};

    particles.emplace_back(p1);

    ParticleContainer pc{particles, {3.,3.,3.}, 1.0};
    auto func = sim::physics::force::FLennardJonesCells(0, 1, 0.1, 1, 1, pc);
    sim::physics::bounds::BoundsHandler bh{sim::physics::bounds::reflecting, sim::physics::bounds::reflecting,
                                           sim::physics::bounds::reflecting, sim::physics::bounds::reflecting,  
                                           sim::physics::bounds::reflecting, sim::physics::bounds::reflecting,
                                           func,
                                           0, 1, 0.1, 1, newSigma,
                                           pc
    };

    pc.forAllParticles([&](Particle& p){
        EXPECT_EQ(p.getF().norm(), 0.)<<"Particle experiences a force even before the boundary condition could be applied";
    });
    bh.operator()();
    
    //apply what should happen with operator manually
    std::array<double, 3> posP1Halo{3. + (3 - p1Copy.getX()[0]), p1Copy.getX()[1], p1Copy.getX()[2]};
    Particle p1CopyHalo{posP1Halo, {0,0,0}, 1, 0};
    func.getForceFunction()(p1Copy, p1CopyHalo);
    
    //now the effect should virtually be the same
    ASSERT_TRUE(std::abs((p1.getX() - p1Copy.getX()).norm()) < p1.getX().norm()*0.000001)<<"Applying boundary condition did not result in the expected applied force";

    
    //test that particle that is just slightly out of range does not get affected by boundary
    pos = {1.99, 1.01, 1.01};
    Particle p2{pos,{0,0,0}, 1, 0};
    particles.clear();
    particles.emplace_back(p2);
    pc = ParticleContainer(particles, {3.0,3.0,3.0}, 1.0);
    
    sim::physics::bounds::BoundsHandler bh2{sim::physics::bounds::reflecting, sim::physics::bounds::reflecting,
                                           sim::physics::bounds::reflecting, sim::physics::bounds::reflecting,  
                                           sim::physics::bounds::reflecting, sim::physics::bounds::reflecting,
                                           func,
                                           0, 1, 0.1, 1, newSigma,
                                           pc
    };

    
    bh2.operator()();
    //nothing should have happened
    pc.forAllParticles([&](Particle& p){
        EXPECT_EQ(p.getF().norm(), 0.)<<"Particle that was out of bounds range still experienced a force";
    });
}