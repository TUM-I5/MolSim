

#include <array>
#include <functional>

#include "particleModel/Particle.h"
#include "utils/ArrayUtils.h"


std::function<std::array<double,3>(const Particle &p_i, const Particle &p_j)> forceSimpleGravitational()
{
    return [](const Particle &p_i, const Particle &p_j) {
        double m_i = p_i.getM(), m_j = p_j.getM();

        std::array<double, 3> x_i = p_i.getX(), x_j = p_j.getX();

        double prefactor = (m_i * m_j) / std::pow(ArrayUtils::L2Norm(x_i - x_j), 3);

        return prefactor * (x_j - x_i);
  };
}


std::function<std::array<double,3>(const Particle &p_i, const Particle &p_j)> forceLennJonesPotentialFunction(double sigma, double epsilon) {
  return [sigma, epsilon](const Particle &p_i, const Particle &p_j) {
    auto x_i = p_i.getX(), x_j = p_j.getX();
    double norm = ArrayUtils::L2Norm(x_i - x_j);

    double prefactor = (-24 * epsilon) / (std::pow(norm, 2));

    prefactor *= (std::pow(sigma / norm, 6) - 2 * std::pow(sigma / norm, 12));

    return prefactor * (x_i - x_j);
  };
}


