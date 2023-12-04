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
     * @param cell_size should be either bigger than the cutoff or the cutoff should be a multiple of the cell_size
     *
     * @throws std::invalid_argument in case the comparing depth is too big for the domain size,
     * in that case the cell size needs to be adjusted for the linked cell algorithm
     */
    CellContainer(double domain_width, double domain_height, double domain_depth, double r_cutoff, double cell_size);

    virtual ~CellContainer();

    class BoundaryIterator;

    class Iterator;


    /**
     * @returns Iterator that iterates over all cells of the container
    */
    Iterator begin();

    Iterator end();


    /**
     * @returns BoundaryIterator that iterates over all cells at the Boundary of the Conainer
    */
    BoundaryIterator begin_boundary();

    BoundaryIterator end_boundary();


    /**
     * @returns Iterator that iterates over all particles that went outside the domain boundary / are in the halo 
     * 
    */
    std::vector<Particle*>::iterator begin_halo();

    std::vector<Particle*>::iterator end_halo();

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
     * stores the particle in particle_instances, and increments particle_amount.
     *
     * @param x_arg particle position
     * @param v_arg particle velocity
     * @param m_arg particle mass
     *
     * @throws std::invalid_argument if the particle position is outside the domain bounds
     */
    void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg) override;


    /**
     * @brief after all Particles were created and are stored, this function creates pointers to them
     * 
     * In the actual cells no Particles, but particle pointer are stored, because the particles
     * need to be frequently moved to other cells and moving pointers is cheaper.
     * 
    */
    void createPointers();

    void plotParticles(outputWriter::VTKWriter &writer) override;



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


    /**
     * Getter(we did not anotate each of those :3):
     * 
    */

    std::array<dim_t, 3>  getDomain_Max(){
        return domain_max_dim;
    }

    bool hasThreeDimensions(){
        return three_dimensions;
    }

    std::array<double,3> getDomainBounds(){
        return domain_bounds;
    }

    double getCellSize(){
        return cell_size;
    }

    std::vector<std::vector<std::vector<std::vector<Particle*>>>>* getParticles() {
        return &particles;
    }

    double getCutOfRadius(){
        return cut_of_radius;
    }

    dim_t getComparingdepth(){
        return comparing_depth;
    }

    std::vector<Particle*>& getHaloParticles(){
        return halo_particles;
    }

    bool is_valid_domain_cell(dim_t x, dim_t y, dim_t z){
        return (1 <= x && x <= domain_max_dim[0] &&  1 <= y && y <= domain_max_dim[1] && 1 <= z && z <= domain_max_dim[2]);
    }


    /**
    * @brief allocates a cell position in the domain for the given position
    *
    * @param x particle position to map a cell position to
    */
    void allocateCell(std::array<double, 3> &x, std::array<dim_t , 3> &cell_position);

private:
    bool three_dimensions;
    const double cell_size;
    double cut_of_radius;
    std::array<dim_t, 3> domain_max_dim;
    std::array<double, 3> domain_bounds;
    dim_t comparing_depth = 1;
    size_t particle_amount = 0;
    std::vector<std::vector<std::vector<std::vector<Particle*>>>> particles;
    std::vector<Particle> particle_instances;
    std::vector<Particle*> halo_particles;

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


    bool isApproximatelyEqual(double a, double b, double epsilon = 1e-8);
};

