//
// Created by Layla Zadina on 09.11.2023.
//

#ifndef PSEMOLDYN_GROUPE_FORMULAS_H
#define PSEMOLDYN_GROUPE_FORMULAS_H

#include <array>


class Formulas {

private:
    double sigma;
    double eps;

public:

    /**
   * L2 norm of (xi, xj)
   */
    static double secondNorm(const std::array<double, 3> &arr);

    /**
   * Lennard-Jones potential
   */
    double calculatePotential(std::array<double, 3> &xi, std::array<double, 3> &xj);

    /**
  * Lennard-Jones force
  */
    std::array<double, 3> calculateLJForce(std::array<double, 3> &xi, std::array<double, 3> &xj);

};


#endif //PSEMOLDYN_GROUPE_FORMULAS_H
