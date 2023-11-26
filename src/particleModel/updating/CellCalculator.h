#pragma once

#include "particleModel/storage/CellContainer.h"

class CellCalculator {

public:
    CellCalculator(CellContainer &cellContainer, const double delta_t);
    
    void calculateF_within();

    void calculateF_linked_cell();

    void calculateV_X_F_within();

    void shiftF();

private:
    CellContainer &cellContainer;
    const double delta_t;

};