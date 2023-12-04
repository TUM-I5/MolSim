

#include <array>
#include <functional>
#include <iostream>

#include "ForceCalculations.h"
#include "particleModel/storage/Particle.h"
#include "utils/ArrayUtils.h"


ForceCalculation forceSimpleGravitational()
{
    return [](const Particle &p_i, const Particle &p_j) {
        double m_i = p_i.getM(), m_j = p_j.getM();

        std::array<double, 3> x_i = p_i.getX(), x_j = p_j.getX();

        double prefactor = (m_i * m_j) / std::pow(ArrayUtils::L2Norm(x_i - x_j), 3);

        return prefactor * (x_j - x_i);
  };
}


ForceCalculation forceLennJonesPotentialFunction(double sigma, double epsilon) {
  return [sigma, epsilon](const Particle &p_i, const Particle &p_j) {
    auto x_i = p_i.getX(), x_j = p_j.getX();
    double norm = ArrayUtils::L2Norm(x_i - x_j);

    //norm = std::max(0.00000001,norm);

    double prefactor = (-24 * epsilon) / (std::pow(norm, 2));

    prefactor *= (std::pow(sigma / norm, 6) - 2 * std::pow(sigma / norm, 12));

    if(std::abs(prefactor) > 10000){
      std::cout << "===================================================apparently very big forces for: \n" << p_i.toString() << " and \n" << p_j.toString() << std::endl;
      std::cout << "========================================" << prefactor << std::endl;
    }


    //prefactor = std::min(prefactor,1000.0);



    return prefactor * (x_i - x_j);
  };
}



ForceCalculation_Ghost forceLennJonesPotentialFunction_Ghost(double sigma, double epsilon){
  return [sigma, epsilon](const Particle &p_i, std::array<double,3> x) {
    auto x_i = p_i.getX();
    double norm = ArrayUtils::L2Norm(x_i - x);

    //norm = std::max(0.00000001,norm);

    double prefactor = (-24 * epsilon) / (std::pow(norm, 2));

    prefactor *= (std::pow(sigma / norm, 6) - 2 * std::pow(sigma / norm, 12));


    return prefactor * (x_i - x);
  };
}



