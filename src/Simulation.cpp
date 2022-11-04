//
// Created by alex on 30.10.2022.
//

#include "Simulation.h"
#include "Particle.h"
#include "ParticleContainer.h"

namespace sim{
    ParticleContainer particleContainer;
    double start_time = 0;
    double end_time;
    double delta_t;

    void calculateFGravity() {
        //set all current forces on all particles to 0
        particleContainer.forAllParticles([](Particle &p) {
            p.setOldF(p.getF());
            p.setF({0., 0., 0.});
        });

        particleContainer.forAllPairs(forceBetw2Particles);
    }

    void calculateXStoermerVelvet() {
        particleContainer.forAllParticles([](Particle &p) {
            Eigen::Vector3d x = delta_t * p.getV() + delta_t * delta_t * p.getOldF() / (2 * p.getM());
            p.add_to_X(x);
        });
    }

    void calculateVStoermerVelvet() {
        particleContainer.forAllParticles([](Particle &p) {
            Eigen::Vector3d v = delta_t * (p.getOldF() + p.getF()) / (2 * p.getM());
            p.add_to_V(v);
        });
    }

    void forceBetw2Particles(Particle &p1, Particle &p2) {
        double delta_x = p1.getX()[0] - p2.getX()[0];
        double delta_y = p1.getX()[1] - p2.getX()[1];
        double scalar =
                p1.getM() * p2.getM() * std::pow(1 / std::sqrt(delta_x * delta_x + delta_y * delta_y), 3);
        double F_X = -delta_x * scalar;
        double F_Y = -delta_y * scalar;
        p1.add_to_F({F_X, F_Y, 0.});
        p2.add_to_F({-F_X, -F_Y, 0.});
    }
} // sim