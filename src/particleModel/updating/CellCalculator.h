#pragma once

#include "particleModel/storage/CellContainer.h"

/**
 * @brief a list of tuples that contain information to change a particles location
 *
 * a tuple contains the index of a particle within it's cell, the current cell it's
 * located and the new cell to move it into.
 */
typedef std::vector<std::tuple<size_t, std::array<dim_t,3>, std::array<dim_t,3>>> instructions;

/**
 * @class CellCalculator
 * @brief offers important functions for particle interactions.
 *
 * This class simulates the interaction between particles inside the cell storage structure.
 * It offers the functionality to calculate the forces, velocities and position of particles, by
 * requesting the next cells to consider, organised by the provided CellContainer, which allows
 * multiple CellCalculators to run in parallel based on the consumer-producer pattern.
 */
class CellCalculator {

public:
    CellCalculator(CellContainer &cellContainer, const double delta_t, const std::string& forceType);


    /**
     * @brief initializes the force and updates the position to remain the calculation order
     *
     * we set the calculation order inside the while loop to F - V - X, that's why we have to
     * calculate X before the loop in order to keep the pattern F - X - F - V - X - F - V - X...
     */
    void initializeFX();

    /**
     * @brief calculate the forces acting between the particles in two cells each, along given paths
     *
     * in order to consider all the particles in cutoff distance, we have to look at a certain amount
     * of cells around a cell depending on the cell size. That amount is determent with the
     * "comparing_depth" attribute, which is the amount of layers we are comparing around each cell.
     * Now instead of iterating through all of the cells within the cutoff radius for each cell directly,
     * we are covering them by iterating over so called "paths" created with a starting position and a
     * shifting pattern that shows which cells should be calculated next.
     * This way we are calculating all the forces within the cutoff radius for each cell, through divide
     * and conquer with "paths" that are non overlapping for each pattern, which allows parallelisation
     * without using locks, with the hope of improving performance.
     */
    void calculateLinkedCellF();

    /**
     * @brief calculate the forces not covered in "calculateLinkedCellF()", V, X and update the cells
     *
     * We applied a new order of calculating because V and X can be calculated independently from
     * other particles as long as the force of them is all up-to-date. This means that after we
     * finished the force calculation within a cell, we can keep the cell and calculate V and X
     * on top. This is more elegant than having to iterate over all cells for X, then F + cell pairs
     * and then iterate over all cells for V again.
     */
    void calculateWithinFVX();

    /**
     * @brief iterate over boundary cells and apply ghost particle force as a boundary condition
     */
    void applyGhostParticles();

private:
    CellContainer &cellContainer;
    const double cell_size;
    const double delta_t;
    double ref_size = std::pow(2, 1.0 / 6);
    std::array<dim_t, 3> domain_max_dim;
    std::vector<std::vector<std::vector<std::vector<Particle*>>>> &particles;
    ForceCalculation forceLambda;
    ForceCalculation_Ghost force;


    void calculateBoundariesTopOrBottom(dim_t z_plane,dim_t z_border);
    void calculateBoundariesFrontOrBack(dim_t x_plane,dim_t x_border,dim_t z_until); //Front and Back
    void calculateBoundariesLeftOrRight(dim_t y_plane,dim_t y_border,dim_t z_until); //Left and Right

    /**
     * @brief helper method to change the location of particles within the cell structure
     *
     * when the positions of the particles get changed a list of instructions is being created
     * that summarizes all the changes of particles between cells that have to be made. That
     * list is being processed in this method.
     *
     * @param cell_updates list of instructions to change the location of particles
     */
    void updateCells(instructions cell_updates);

    /**
     * @brief helper method calculate the Velocity and Position
     *
     * in order to keep "calculateWithinFVX()" and "initializeFX()" clean, we moved
     * the functionality to calculate the two attributes V and X into this method.
     *
     * @param particle the particle to update the attributes of
     * @param current_position the current cell the particle is located in
     * @param cell_updates the list of instructions in case the particle changes its current position
     * @param calculateV to make it applicable for both calculations "FVX" and "FX"
     * @param particle_index the index of the particle within the cell for potential cell updates
     */
    void calculateVX(Particle& particle, std::array<dim_t, 3> &current_position,
                     instructions &cell_updates, bool calculateV, size_t particle_index);

    /**
     * @brief helper method to calculate the forces not covered in "calculateLinkedCellF()"
     *
     * since the "calculateLinkedCellF()" methods only calculates the forces between cells, we are
     * now calculating the forces between the particles within a cell.
     *
     * @param current_cell the cell to calculate the force within
     */
    void finishF(std::vector<Particle*> &current_cell);
};