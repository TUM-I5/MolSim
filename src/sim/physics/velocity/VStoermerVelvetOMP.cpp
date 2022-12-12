//
// Created by alex on 26.11.2022.
//

#include "VStoermerVelvetOMP.h"

namespace sim::physics::velocity {
    void VStoermerVelvetOMP::operator()() {
        double delta_t = this->delta_t;
        particleContainer.runOnData([delta_t](std::vector<double> &force,
                                       std::vector<double> &oldForce,
                                       std::vector<double> &x,
                                       std::vector<double> &v,
                                       std::vector<double> &m,
                                       std::vector<int> &type,
                                       unsigned long count, auto, auto) {
#pragma omp parallel default(none) shared(force, oldForce, v, m, count, delta_t)
            {
#pragma omp for
                for (unsigned long index = 0; index < count; index++) {
                    v[index*3 + 0] += delta_t * (oldForce[index*3 + 0] + force[index*3 + 0]) / (2 * m[index]);
                    v[index*3 + 1] += delta_t * (oldForce[index*3 + 1] + force[index*3 + 1]) / (2 * m[index]);
                    v[index*3 + 2] += delta_t * (oldForce[index*3 + 2] + force[index*3 + 2]) / (2 * m[index]);
                }
            }
        });
    }
} //sim::physics::velocity