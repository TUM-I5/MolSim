#include <gtest/gtest.h>

#include "data/Particle.h"
#include "io/output/Logging.h"
#include "data/ParticleContainer.h"
#include "sim/physics/force/FLennardJonesCells.h"

#include <Eigen>
#include <vector>

ParticleContainer pcWithParticles(std::array<double, 3> p1Coord, std::array<double, 3> p2Coord){
    std::vector<Particle> buffer{};
    Particle p1{p1Coord, {0.,0.,0.}, 1., 0};
    Particle p2{p2Coord, {0.,0.,0.}, 1., 0};
    buffer.emplace_back(p1);
    buffer.emplace_back(p2);
    return ParticleContainer{buffer, {3.1,3.1,3.1}, 1.2};
}

/**
 * @brief Test if LennardJonesCells lets the right particles interact with each other
 * 
 * 
 */
TEST(FLennardJonesCells, operator){
    spdlog::set_level(static_cast<spdlog::level::level_enum>(6));
    //get two particle in the same cell
    auto pc = pcWithParticles({0.5,0.5,0.5}, {1.,1.,1.});

    auto func = sim::physics::force::FLennardJonesCells(0, 1, 0.1, 1, 1, pc);
    func.operator()();

    //now you should have interacted
    pc.forAllParticles([&](Particle& p){
        ASSERT_TRUE(p.getF().norm() != 0);
    });

    //make 2 particles in neighbouring cells
    pc = pcWithParticles({0.5,0.5,0.5}, {1.9,1.,1.});

    func.operator()();

    //you should interact
    pc.forAllParticles([&](Particle& p){
        ASSERT_TRUE(p.getF().norm() != 0);
    });

    //2 particles in diagonally neighbouring cells
    pc = pcWithParticles({0.5,0.5,0.5}, {1.9,1.9,1.});
    func.operator()();

    //you should interact
    pc.forAllParticles([&](Particle& p){
        ASSERT_TRUE(p.getF().norm() != 0);
    });

    //2 particles in not neighbouring
    pc = pcWithParticles({0.5,0.5,0.5}, {3.,1.,1.});

    func.operator()();

    //you should not interact
    pc.forAllParticles([&](Particle& p){
        ASSERT_TRUE(p.getF().norm() == 0.)<< "Two particles in cells that weren't neighbours applied a force of " <<p.getF().norm()<<" to each other";
    });
}
