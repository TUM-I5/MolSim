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
    double ref_size = std::pow(2, 1.0 / 6);
    std::vector<std::vector<std::vector<std::vector<Particle*>>>> &particles;
    ForceCalculation_Ghost force;


    void calculateBoundariesTopOrBottom(dim_t z_plane,dim_t z_border);
    void calculateBoundariesFrontOrBack(dim_t x_plane,dim_t x_border,dim_t z_until); //Front and Back
    void calculateBoundariesLeftOrRight(dim_t y_plane,dim_t y_border,dim_t z_until); //Left and Right

    void updateCells(instructions cell_updates);

    void calculateVX(Particle& particle, std::array<dim_t, 3> &current_position,
                     instructions &cell_updates, bool calculateV, size_t particle_index);

    void finishF(std::vector<Particle*> &current_cell);
};