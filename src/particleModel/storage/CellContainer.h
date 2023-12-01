#pragma once


#include "utils/ForceCalculations.h"
#include "Particle.h"
#include "outputWriter/VTKWriter.h"
#include <vector>

typedef int dim_t;
extern dim_t dim_t_res;

class CellContainer {
public:
    CellContainer(double domain_width, double domain_height, double domain_depth, double r_cutoff, double cell_size, const std::string& forceType);

    virtual ~CellContainer();


/**
 * @class BoundaryIterator
 * @brief Iterator that returns all boundary cells of the CellContainer
 */
class BoundaryIterator {
public:
    /** the first cell is on the left lower corner of a cuboid/ rectangle domain*/
    dim_t x = 1; 
    dim_t y = 1; 
    dim_t z = 1; 
    CellContainer& cell; /** the Container over which boundary cells the iterator iterates*/

    /**
     * @brief Constructor initializing dimensions and CellContainer reference.
     * @param cell_cont Reference to the CellContainer
     */
    BoundaryIterator(CellContainer& cell_cont) : cell(cell_cont) {};

    /**
     * @brief Constructor initializing dimensions and CellContainer reference.
     * @param cell_cont Reference to the CellContainer
     * @param x_ X cell dimension from which to start
     * @param y_ Y cell dimension from which to start
     * @param z_ Z cell dimension from which to start
     */
    BoundaryIterator(CellContainer& cell_cont, dim_t x_, dim_t y_, dim_t z_) : x(x_), y(y_), z(z_), cell(cell_cont) {};

    /**
     * @brief Prefix increment operator.
     * @return Reference to the incremented iterator.
     */
    BoundaryIterator& operator++();

    /**
     * @brief Dereference operator to obtain a vector of particles.
     * @return Reference to the vector of particles.
     */
    std::vector<Particle*>& operator*();

    /**
     * @brief Equality operator.
     * @param other Another BoundaryIterator to compare with.
     * @return True if both iterators are currently "pointing" to the same cell. (same x, y , z)
     */
    bool operator==(const BoundaryIterator& other);

    /**
     * @brief Inequality operator.
     * @param other Another BoundaryIterator to compare with.
     * @return True if iterators are not equal "pointing" to the same cell. (same x, y , z)
     */
    bool operator!=(const BoundaryIterator& other);
};

    BoundaryIterator begin_boundary();

    BoundaryIterator end_boundary();


    void setNextCell(std::array<dim_t, 3> &next_position);

    void setNextPath(std::array<dim_t, 3> &start, std::array<dim_t, 3> &pattern);

    void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg);

    void plotParticles(outputWriter::VTKWriter &writer);

    std::array<dim_t, 3> getDomain_Max();

    bool getThreeDimensions(){
        return three_dimensions;
    }

    std::array<double,3> getDomainBounds(){
        return domain_bounds;
    }

    /**
     * @brief returns the string representation of the CellContainer
     * @returns string representation
    */
    std::string to_string();

    /**
     * @brief returns size of the CellContainer
     * @returns size of the CellContainer
    */
    size_t size();

    double getCellSize();

    auto getParticles() {
        return &particles;
    }

private:
    bool three_dimensions;
    const double cell_size;
    std::array<dim_t, 3> domain_max_dim;
    std::array<double, 3> domain_bounds;
    dim_t comparing_depth = 1;
    size_t particle_amount = 0;
    ForceCalculation forceLambda;
    std::vector<std::vector<std::vector<std::vector<Particle*>>>> particles;
    std::vector<Particle> particle_instances;

    void setNext3dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start);

    void setNext2dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start);

    void allocateCell(std::array<double, 3> &x, std::array<dim_t , 3> &cell_position);

    void next_correct_boundary_index(dim_t& x,dim_t& y,dim_t& z);
};

