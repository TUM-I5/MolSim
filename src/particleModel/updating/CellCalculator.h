#pragma once

#include "particleModel/storage/CellContainer.h"

typedef std::vector<std::tuple<size_t, std::array<dim_t,3>, std::array<dim_t,3>>> instructions;

class CellCalculator {

public:
    CellCalculator(CellContainer &cellContainer, const double delta_t);

    void initializeFX();

    void calculateLinkedCellF();

    void calculateWithinFVX();

    void applyGhostParticles();

private:
    CellContainer &cellContainer;
    const double cell_size;
    const double delta_t;
    std::vector<std::vector<std::vector<std::vector<Particle*>>>> &particles;

    void updateCells(instructions cell_updates);

    void calculateVX(Particle& particle, std::array<dim_t, 3> &current_position,
                     instructions &cell_updates, bool calculateV, size_t particle_index);

    void finishF(std::vector<Particle*> &current_cell);
};