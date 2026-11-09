#pragma once

#include "utils/ArrayUtils.h"
#include "utils/Vec3.h"

/**
 * @brief Lennard Jones force kernel to get the force between two particles
 */
struct LennardJones {
    double epsilon = 5.0;  ///< Depth of the potential well (default from worksheet)
    double sigma = 1.0;    ///< Distance where the potential is 0

    /**
     * @brief Computes force vector
     *
     * @param p_i Particle the force acts on
     * @param p_j Particle exerting the force
     * @return Force vector on @p p_i by @p p_j
     */
    Vec3 operator()(auto p_i, auto p_j) const {
        const Vec3 delta_x = p_j.x() - p_i.x();
        const double r2 = ArrayUtils::L2NormSquared(delta_x);
        const double inv_r2 = 1.0 / r2;
        const double s2 = sigma * sigma * inv_r2;
        const double s6 = s2 * s2 * s2;
        const double s12 = s6 * s6;
        return (24.0 * epsilon * inv_r2 * (s6 - 2.0 * s12)) * delta_x;
    }
};
