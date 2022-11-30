//
// Created by alex on 26.11.2022.
//

#include "FGravity.h"

namespace sim::physics::force {
    void FGravity::operator()() {
        //set all current forces on all particles to 0
        particleContainer.forAllParticles([&](Particle &p) {
            p.setOldF(p.getF());
            p.setF({0., 0., 0.});
        });

        particleContainer.forAllPairs(pairFun);
    }

    pair_fun_t &FGravity::getForceFunction() {
        return pairFun;
    }

    void FGravity::setPairFun() {
        pairFun = [&](Particle &p1, Particle &p2) {
            double delta_x = p1.getX()[0] - p2.getX()[0];
            double delta_y = p1.getX()[1] - p2.getX()[1];
            double scalar =
                    p1.getM() * p2.getM() * std::pow(1 / std::sqrt(delta_x * delta_x + delta_y * delta_y), 3);
            double F_X = -delta_x * scalar;
            double F_Y = -delta_y * scalar;
            p1.add_to_F({F_X, F_Y, 0.});
            p2.add_to_F({-F_X, -F_Y, 0.});
        };
    }

    void FGravity::setParticleContainer(ParticleContainer &pc) {
        PhysicsFunctorBase::setParticleContainer(pc);
        setPairFun();
    }
} // sim::physics::force