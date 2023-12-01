#pragma once

#include "SimulationContainer.h"
#include "utils/ForceCalculations.h"
#include "Particle.h"
#include "outputWriter/VTKWriter.h"
#include <vector>

/**
 * @brief type for dimensions, used to apply changes easily in the code
 */
typedef int dim_t;

/**
 * @brief reserved dim_t value for controlling
 */
extern dim_t dim_t_res;

/**
 * @class CellContainer
 * @brief container structure to store the particles within cells
 */
class CellContainer : public SimulationContainer {
public:
    /**
     * @brief constructor initializing the cell storage structure and the maximum domain cell index
     *
     * the cell storage structure contains the halo cells. Also the dimension will be set for the
     * linked cell algorithm and the comparing depth is being determined based on the cell size and
     * cutoff radius.
     *
     * @param domain_width width of the domain
     * @param domain_height height of the domain
     * @param domain_depth depth of the domain, set smaller than cell size to create 2d structure
     * @param r_cutoff cutoff to indicate when particles too far away can be ignored
     * @param cell_size should be either bigger than or a multiple of the cutoff
     */
    CellContainer(double domain_width, double domain_height, double domain_depth, double r_cutoff, double cell_size);

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

    /**
     * @brief provides an iteration over the non empty domain cells based on the consumer-producer pattern
     *
     * each call sets one iteration into "next_position" until all the domain cells got covered.
     * Indicates the end with the reserved value "dim_t_res".
     * Implemented for parallelisation-friendliness.
     *
     * @param next_position array to set the next position or "dim_t_res" to.
     */
    void setNextCell(std::array<dim_t, 3> &next_position);

    /**
     * @brief provides an iteration over the paths that sum up to all the linked cell combinations
     *
     * each call sets one "start" and one "pattern" into the given parameters, this is called by the
     * "calcualteLinkedCellF()" method in the "CellCalculator" class. Read there to get details of
     * the underlying algorithm.
     * This method creates non-overlapping "paths" that can be processed simultaneously per pattern,
     * thereby allowing parallelisation without using locks, with the hope of improving performance.
     * It indicates the end of the iterations by setting "start[0]" value to reserved value "dim_t_res".
     *
     * @param start array to set the start position to or indicate the end
     * @param pattern array to set the pattern in which to move across the cells
     */
    void setNextPath(std::array<dim_t, 3> &start, std::array<dim_t, 3> &pattern);

    /**
     * @brief creates a particle instance with the given parameters
     *
     * stores the particle in particle_instances, adds a pointer to the cell structure with the
     * corresponding position and increments particle_amount.
     *
     * @param x_arg particle position
     * @param v_arg particle velocity
     * @param m_arg particle mass
     *
     * @throws std::invalid_argument if the particle position is outside the domain bounds
     */
    void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg) override;

    void plotParticles(outputWriter::VTKWriter &writer) override;

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
    std::string to_string() override;

    /**
     * @brief returns size of the CellContainer
     * @returns size of the CellContainer
    */
    size_t size() override;


    double getCellSize();

    /**
     * @brief returns reference to the cell structure storing particle pointers
     */
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
    std::vector<std::vector<std::vector<std::vector<Particle*>>>> particles;
    std::vector<Particle> particle_instances;

    /**
      * @brief helper method to set the next 3d pattern for "setNextPath(...)"
      *
      * @param pattern to set the next pattern to
      * @param start to set the reserved value in case all patterns are done
      */
    void setNext3dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start);

    /**
      * @brief helper method to set the next 2d pattern for "setNextPath(...)"
      *
      * @param pattern to set the next pattern to
      * @param start to set the reserved value in case all patterns are done
      */
    void setNext2dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start);

    /**
      * @brief allocates a cell position in the domain for the given position
      *
      * @param x particle position to map a cell position to
      */
    void allocateCell(std::array<double, 3> &x, std::array<dim_t , 3> &cell_position);

    void next_correct_boundary_index(dim_t& x,dim_t& y,dim_t& z);
};

