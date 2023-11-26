#pragma once

#include "particleModel/storage/CellContainer.h"

class CellCalculator {

public:
    CellCalculator(CellContainer &cellContainer, const double delta_t);

    void initializeF();

    void calculateLinkedCellF();

    void calculateWithinVXF();

private:
    CellContainer &cellContainer;
    const double delta_t;
    std::vector<std::vector<std::vector<std::unordered_set<Particle, ParticleHashing>>>> &particles;
};