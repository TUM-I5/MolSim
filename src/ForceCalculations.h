#pragma once

#include <array>
#include "Particle.h"
#include "utils/ArrayUtils.h"




std::array<double, 3> forceSimpleGravitational(const Particle &p_i,const Particle &p_j, double,double)
{
    auto m_i = p_i.getM(), m_j = p_j.getM();

    auto x_i = p_i.getX(), x_j = p_j.getX();

    double prefactor = (m_i * m_j) / std::pow(ArrayUtils::L2Norm(x_i - x_j), 3);

    return prefactor * (x_j - x_i);
}




std::array<double, 3> forceLennJonesPotential(const Particle &p_i,const Particle &p_j, double sigma, double epsilon)
{
    auto x_i = p_i.getX(), x_j = p_j.getX();
    double norm = ArrayUtils::L2Norm(x_i - x_j);

    auto prefactor = (-24 * sigma)/(std::pow(norm,2));

    prefactor *= (
        std::pow(sigma/norm,6)  -   2* std::pow(sigma/norm,12)
    );

    return prefactor * (x_j - x_i);
}