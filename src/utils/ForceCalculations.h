#pragma once

#include <array>
#include <functional>

#include "particleModel/Particle.h"
#include "utils/ArrayUtils.h"

/**
 * @file
 * 
 * contains the different functions that can be used for calculation of
 * forces between two concrete Particles, all functions share the same
 * function interface, such that they can be passed as arguments
 * to the calulateF() function, that calculates Forces between not only two, but
 * all Particles
 */




/**
 * @brief Calculate force between \f$ p_i \f$ and \f$ p_j \f$
 *
 * Uses particle \f$ p_i \f$ and particle \f$ p_j \f$ to calculate the
 * force \f$ f_{ij} \f$ between them.
 * Uses the formula from the first worksheet:
 * \f$ F_{ij} = \frac{m_i m_j}{{\| \mathbf{x}_i - \mathbf{x}_j \|}^3}
 * (\mathbf{x}_j - \mathbf{x}_i) \f$
 *
 *
 * @param p_i Particle \f$ i \f$ for force calculation
 * @param p_j Particle \f$ j \f$ for force calculation
 *
 * @return Three-dimensional vector that corresponds to \f$ f_{ij} \f$
 */
std::function<std::array<double,3>(const Particle &p_i, const Particle &p_j)> forceSimpleGravitational();


/**
 * @brief Calculate force between \f$ p_i \f$ and \f$ p_j \f$
 *
 * Uses particle \f$ p_i \f$ and particle \f$ p_j \f$ to calculate the
 * force \f$ f_{ij} \f$ between them.
 * Uses the formula from the second worksheet:
 * \f$ F_{ij} = -\frac{24 \varepsilon}{{\| \mathbf{x}_i - \mathbf{x}_j \|}^2} \left(
 * \left( \frac{\sigma}{{\| \mathbf{x}_i - \mathbf{x}_j \|}} \right)^{6} - 2
 * \left( \frac{\sigma}{{\| \mathbf{x}_i - \mathbf{x}_j \|}} \right)^{12}
 * \right) (\mathbf{x}_i - \mathbf{x}_j) \f$
 *
 *
 * @param p_i Particle \f$ i \f$ for force calculation
 * @param p_j Particle \f$ j \f$ for force calculation
 *
 * @return Three-dimensional vector that corresponds to \f$ f_{ij} \f$
 */
std::function<std::array<double,3>(const Particle &p_i, const Particle &p_j)> forceLennJonesPotentialFunction(double sigma, double epsilon);


