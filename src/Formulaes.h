//
// Created by Layla Zadina on 09.11.2023.
//

#ifndef PSEMOLDYN_GROUPE_FORMULAES_H
#define PSEMOLDYN_GROUPE_FORMULAES_H

#include <array>
#include "utils/ArrayUtils.h"



class Formulaes {

public:


    /**
   * L2 norm of (xi, xj)
   */
    static double secondNorm(const std::array<double, 3>& arr);

    /**
   * Lennard-Jones potential
   */
    double calculatePotential(std::array<double, 3> &xi, std::array<double, 3> &xj);

    /**
  * Lennard-Jones force
  */
    static auto calculateLJForce(std::array<double, 3> &xi, std::array<double, 3> &xj);

};


#endif //PSEMOLDYN_GROUPE_FORMULAES_H
