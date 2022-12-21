//
// Created by alex on 26.11.2022.
//

#include "VStoermerVelvet.h"

namespace sim::physics::velocity {
    void VStoermerVelvet::operator()() {
#ifdef slow
        particleContainer.forAllParticles([&](Particle &p) {
            Eigen::Vector3d v = delta_t * (p.getOldF() + p.getF()) / (2 * p.getM());
            p.add_to_V(v);
        });
#else
        particleContainer.runOnData([&](std::vector<double> &force, std::vector<double> &oldForce,
                                        std::vector<double> &x, std::vector<double> &v,
                                        std::vector<double> &m, std::vector<int> &type,
                                        unsigned long count, std::vector<double> &eps, std::vector<double> &sig){
            for (unsigned long index = 0; index < count; ++index) {
                v[3*index + 0] += delta_t * (oldForce[3*index + 0] + force[3*index + 0]) / (2 * m[index]);
                v[3*index + 1] += delta_t * (oldForce[3*index + 1] + force[3*index + 1]) / (2 * m[index]);
                v[3*index + 2] += delta_t * (oldForce[3*index + 2] + force[3*index + 2]) / (2 * m[index]);
            }
        });
#endif
    }
} //sim::physics::velocity