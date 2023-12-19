#pragma once

#include "particleModel/storage/CellContainer.h"
#include <optional>

enum class boundary_conditions{
    outflow,
    reflective,
    ghost_reflective,
    periodic
};

extern double min_distance;

extern std::vector<std::vector<double>> sigma_mixed;

extern std::vector<std::vector<double>> epsilon_mixed;

/**
 * @brief a list of tuples that contain information to change a particles location
 *
 * a tuple contains the index of a particle within it's cell, the current cell it's
 * located and the new cell to move it into.
 */
typedef std::vector<std::tuple<Particle*, std::array<dim_t,3>>> instructions;

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
    CellCalculator(CellContainer &cellContainer, double delta_t, double cutoff,
                   std::array<boundary_conditions,6> boundaries_cond, double initial_temp = -1,
                   std::optional<double> target_temp_param = std::nullopt, 
                   std::optional<double> max_temp_diff_param = std::nullopt,
                   double gravity_factor = 0);

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
     * 
     * @brief calculates boundary conditions 
     * 
     * This method assumes, that at the moment it is called all particles are within the 
     * domain boundaries. If a particle has coordinates outside the domain it is undefined behaviour.
     * applyBoundaries iterates over every side of the cuboid (that represents the valid domain) and 
     * applies boundary conditions the boundary conditions specified in the 'boundaries' member
     * of the CellCalculator class.
     * If we are not simulating with three dimensions, the claculation of boundary conditions
     * for the top and bottom of the domain do not happen (positive/ negative z direction).
     * 
     * If the boundary conditions are 'outflow', then just no 
     * boundary conditions are applied in that direction.
     * The outflow is automatically realized by updateCells, which deletes
     * Particles from the cellContainer, if they have moved outside the boundary.
     * (the particles are moved into the halo_particles vector)
     * 
     * If the boundary conditions are reflective, applyBoundaries uses Ghost Partilces to create
     * opposing forces for particles in the direction of that boundary side. 
     * 
     * (for imagination(how i think):
     * 
     *  z ^   ^
     *    |  / x
     *    | /
     *    |/_________ > y
     * )
     * 
     * 
     * 
    */
    void applyReflectiveBoundaries();


    void applyThermostats();



    auto& getParticles(){
        return particles;
    }

    auto getDomain_Max(){
        return domain_max_dim;
    }

    auto getDomainBounds(){
        return domain_bounds;
    }

    std::array<double,3> force(const Particle &p_i, const Particle &p_j, const std::array<double,3> &offset);

    std::array<double,3> ghostParticleLennardJonesForce(const Particle &particle,std::array<double,3> ghost_position);


private:
    CellContainer &cellContainer;
    const double gravity_factor;
    const double delta_t;
    const double cutoff;
    std::array<dim_t, 3> domain_max_dim;
    std::array<double,3> domain_bounds;

    double initial_temp;
    std::optional<double> max_temp_diff;
    std::optional<double> target_temp;

    //{positive_z,negative_z,positive_x,negative_x,positive_y,negative_y}
    std::array<boundary_conditions,6> boundaries;
    bool ghost_reflection_is_on;

    std::vector<std::vector<std::vector<std::vector<Particle*>>>> &particles;

    void addGhostParticleForcesInDir_i(int i,double boundary,
                                    std::vector<Particle*>& particles);

    void removeParticlesInDir_i(int i,double boundary,
                                            std::vector<Particle*>& cell);

        /**
     * @brief iterates of the Top or the Bottom side of the domain
     * 
     * (for imagination(how i think):
     * 
     *  z ^   ^
     *    |  / x
     *    | /
     *    |/_________ > y
     * )
     * 
     * This function iterates over cells of cellContainer within the domain.
     * Depending on lower_z and upper_z iterates either over Top or Bottom side of domain.
     * iterates for a fixed z over the plane 
     * {
     *  1(minimum domain cell in y direction) until domain_max_dim[1](maximum domain cell in y direction),
     *  1(minimum domain cell in x direction) until domain_max_dim[0](maximum domain cell in x direction)
     * }
     * (this plane corresponds to one layer of cells -> iterates over all these cells)
     * iterates over a plane of this form for every z in [lower_z,upper_z]  (both are inclusive borders)
     * 
     * -> for every plane (horizontal plane of cells) all Particles in the plane of cells are iterated 
     * and it is checked if they are closer than sigma * 2^(1/6) to
     * 'z_border' (either lower domain border or upper domain border). 
     * If a Particle p1 is close enough, forces are calculated between this Particle p1 and a Particle, that 
     * has a position that corresponds to the position of p1, but mirrored at z_border.
     *  
     * @param lower_z lower bound for the x-y-planes of cells that are iterated
     * @param upper_z upper bound for the x-y-planes of cells that are iterated
     * @param z_border real valued domain boundary, forces are calculated relative to this border
     * 
     * 
    */
    void calculateBoundariesTopOrBottom(dim_t lower_z,dim_t upper_z,double z_border);


        /**
     * @brief iterates of the Front or the Back side of the domain
     * 
     * (for imagination(how i think):
     * 
     *  z ^   ^
     *    |  / x
     *    | /
     *    |/_________ > y
     * )
     * 
     * 
     * This function iterates over cells of cellContainer within the domain.
     * Depending on lower_x and upper_x iterates either over Front or Back side of domain.
     * iterates for a fixed x over the plane 
     * {
     *  1(minimum domain cell in y direction) until domain_max_dim[1](maximum domain cell in y direction),
     *  1(minimum domain cell in z direction) until domain_max_dim[2](maximum domain cell in z direction)
     * }
     * (this plane corresponds to one layer of cells -> iterates over all these cells)
     * iterates over a plane of this form for every x in [lower_x,upper_x]  (both are inclusive borders)
     * 
     * -> for every plane (vertical plane of cells) all Particles in the plane of cells are iterated 
     * and it is checked if they are closer than sigma * 2^(1/6) to
     * 'x_border' (either lower domain border or upper domain border). 
     * If a Particle p1 is close enough, forces are calculated between this Particle p1 and a Particle, that 
     * has a position that corresponds to the position of p1, but mirrored at x_border.
     *  
     * Additionaly for 2D boundaries have the parameter z_until, that ensures, that not the whole
     * plane 
     * {
     *  1(minimum domain cell in y direction) until domain_max_dim[1](maximum domain cell in y direction),
     *  1(minimum domain cell in z direction) until domain_max_dim[2](maximum domain cell in z direction)
     * }
     * is iterated, but only 
     * {
     *  1(minimum domain cell in y direction) until domain_max_dim[1](maximum domain cell in y direction),
     *  1 until z_until (z_until will be 1 for 2D Boundary Conditions)
     * }
     * 
     * @param lower_x lower bound for the y-z-planes of cells that are iterated
     * @param upper_x upper bound for the y-z-planes of cells that are iterated
     * @param x_border real valued domain boundary, forces are calculated relative to this border
     * @param z_until determines how much of the plane will be calculated
     * 
     * 
    */
    void calculateBoundariesFrontOrBack(dim_t lower_x,dim_t upper_x ,double x_border,dim_t z_until); //Front and Back

    /**
     * @brief iterates of the Left or the Right side of the domain
     * 
     * 
     *(for imagination(how i think):
    * 
    *  z ^   ^
    *    |  / x
    *    | /
    *    |/_________ > y
    * ) 
    *
    * 
    * This function iterates over cells of cellContainer within the domain.
    * Depending on lower_y and upper_y iterates either over Left  or Right  side of domain.
    * iterates for a fixed y over the plane 
    * {
    *  1(minimum domain cell in x direction) until domain_may_dim[0](maximum domain cell in x direction),
    *  1(minimum domain cell in z direction) until domain_max_dim[2](maximum domain cell in z direction)
    * }
    * (this plane corresponds to one layer of cells -> iterates over all these cells)
    * iterates over a plane of this form for every y in [lower_y,upper_y]  (both are inclusive borders)
    * 
    * -> for every plane (also vertical plane of cells) all Particles in the plane of cells are iterated 
    * and it is checked if they are closer than sigma * 2^(1/6) to
    * 'y_border' (will be either lower domain border or upper domain border). 
    * If a Particle p1 is close enough, forces are calculated between this Particle p1 and a Particle, that 
    * has a position that corresponds to the position of p1, but mirrored at y_border.
    *  
    * Additionaly for 2D boundaries have the parameter z_until, that ensures, that not the whole
    * plane 
    * {
    *  1(minimum domain cell in x direction) until domain_may_dim[0](maximum domain cell in x direction),
    *  1(minimum domain cell in z direction) until domain_max_dim[2](maximum domain cell in z direction)
    * }
    * is iterated, but only 
    * {
    *  1(minimum domain cell in x direction) until domain_max_dim[0](maximum domain cell in x direction),
    *  1 until z_until (z_until will be 1 for 2D Boundary Conditions)
    * }
    * 
    * @param lower_y lower bound for the x-z-planes of cells that are iterated
    * @param upper_y upper bound for the x-z-planes of cells that are iterated
    * @param y_border real valued domain boundary, forces are calculated relative to this border
    * @param z_until determines how much of the plane will be calculated
    * 
    * 
    */
    void calculateBoundariesLeftOrRight(dim_t lower_y,dim_t upper_y ,double y_border,dim_t z_until); //Left and Right

    /**
     * @brief helper method to change the location of particles within the cell structure
     *
     * when the positions of the particles get changed a list of instructions is being created
     * that summarizes all the changes of particles between cells that have to be made. That
     * list is being processed in this method.
     *
     * @param cell_updates list of instructions to change the location of particles
     */
    void updateCells(instructions& cell_updates);

    /**
     * @brief helper method calculate the Velocity and Position
     *
     * in order to keep "calculateWithinFVX()" and "initializeFX()" clean, we moved
     * the functionality to calculate the two attributes V and X into this method.
     *
     * @param particle the particle to update the attributes of
     * @param calculateV to make it applicable for both calculations "FVX" and "FX"
     */
    void calculateVX(Particle& particle, bool calculateV);

    /**
     * @brief helper method to calculate the forces not covered in "calculateLinkedCellF()"
     *
     * since the "calculateLinkedCellF()" methods only calculates the forces between cells, we are
     * now calculating the forces between the particles within a cell.
     *
     * @param current_cell the cell to calculate the force within
     */
    void finishF(std::vector<Particle*> *current_cell);

    bool inCutoffDistance(Particle &p1, Particle &p2, const std::array<double,3> &offset) const;

    bool mirror(std::array<dim_t,3> &position, std::array<double,3> &offset);
};