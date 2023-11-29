#pragma once

#include "particleModel/storage/CellContainer.h"

class CellCalculator {

public:
    CellCalculator(CellContainer &cellContainer, const double delta_t);

    void initializeFX();

    void calculateLinkedCellF();

    void calculateWithinFVX();

private:
    CellContainer &cellContainer;
    const double delta_t;
    std::vector<std::vector<std::vector<std::vector<Particle>>>> &particles;
};