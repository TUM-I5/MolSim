#pragma once

#include "CellContainer.h"

typedef int dim_t;



/**
 * @class CustomIterator
 * @brief iterates over a custom subcuboid of all cells 
 * 
 * 
 * The cellContainer is a cuboid of cells, the cells are:
 *                            { (n1,n2,n3) \in N^3 | 0 <=  n1  <= domain_max_dim[0]+2 
 *                                              \and 0 <= n2 <= domain_max_dim[1]+2 
 *                                              \and 0 <= n3 <= domain_max_dim[2]+2}
 * where N are the natural numbers
 *
 * The CustomIterator can be created with a custom lower/ upper bound for 
 * the x,y and the z direction and the iterates over all these cells:
 *                            { (n1,n2,n3) \in N^3 | lower_x <=  n1  <= upper_x 
 *                                              \and lower_y <=  n2  <= upper_y 
 *                                              \and lower_z <=  n3  <= upper_z}
 * where N are the natural numbers
 * 
 * This way one can iterate over an arbitrary subcuboid of the CellContainer
 * 
 */
class CellContainer::CustomIterator{
public:
    dim_t lower_x = 1; /**< Lower bound for x dimension */
    dim_t upper_x; /**< Upper bound for x dimension */
    dim_t lower_y = 1; /**< Lower bound for y dimension */
    dim_t upper_y; /**< Upper bound for y dimension */
    dim_t lower_z = 1; /**< Lower bound for z dimension */
    dim_t upper_z; /**< Upper bound for z dimension */

    dim_t x = 1; /**< Current x dimension */
    dim_t y = 1; /**< Current y dimension */
    dim_t z = 1; /**< Current z dimension */

    CellContainer& cell; /** the Container over which boundary cells the iterator iterates*/

    /**
     * @brief Constructor initializing dimensions and CellContainer reference.
     * @param cell_cont Reference to the CellContainer
     */
    CustomIterator(CellContainer& cell_cont) : cell(cell_cont) {};


     /**
     * @brief Constructor initializing dimensions and CellContainer reference for iterating over a custom subcuboid of cells.
     *
     * @param cell_cont Reference to the CellContainer
     * @param low_x Lower bound for x dimension within the custom subcuboid
     * @param upp_x Upper bound for x dimension within the custom subcuboid
     * @param low_y Lower bound for y dimension within the custom subcuboid
     * @param upp_y Upper bound for y dimension within the custom subcuboid
     * @param low_z Lower bound for z dimension within the custom subcuboid
     * @param upp_z Upper bound for z dimension within the custom subcuboid
     *
     * The Iterator created by this Constructor will iterate over the following cells 
     * - { (n1,n2,n3) ∈ N^3 | lower_x <=  n1  <= upper_x
     *                        ∧ lower_y <=  n2  <= upper_y
     *                        ∧ lower_z <=  n3  <= upper_z }
     * 
     * begining with the cell (low_x,low_y,low_z)
     * (where N represents the set of natural numbers.)
     * 
     * This constructor enables iteration over an arbitrary subcuboid of the CellContainer.
     */
    CustomIterator(CellContainer& cell_cont, dim_t low_x, dim_t upp_x 
                                           , dim_t low_y, dim_t upp_y
                                           , dim_t low_z, dim_t upp_z) 
    : lower_x(low_x), upper_x(upp_x) , lower_y(low_y) , upper_y(upp_y) ,
      lower_z(low_z) , upper_z(upp_z) , x(low_x) , y(low_y), z(low_z) , cell(cell_cont)  {};



    /**
     * @brief Prefix increment operator.
     * @return Reference to the incremented iterator.
     */
    CustomIterator& operator++();

    /**
     * @brief Dereference operator to obtain a vector of particles.
     * @return Reference to the vector of particles.
     */
    std::vector<Particle*>& operator*();

    /**
     * @brief Equality operator.
     * @param other Another CustomIterator to compare with.
     * @return True if both iterators are currently "pointing" to the same cell. (same x, y , z)
     */
    bool operator==(const CustomIterator& other);

    /**
     * @brief Inequality operator.
     * @param other Another CustomIterator to compare with.
     * @return True if iterators are not equal "pointing" to the same cell. (same x, y , z)
     */
    bool operator!=(const CustomIterator& other);



private:
    /**
     * @brief internal method to find next correct index
     * 
    */
    void next_correct_index_custom(dim_t& x, dim_t& y, dim_t& z);

};

/**
 * @class Iterator
 * @brief iterates over all cells of the CellContainer, that are in the domain
 *
 * iterates over the cells in the set:
 *                            { (n1,n2,n3) \in N^3 | 1 <=  n1  <= domain_max_dim[0] 
 *                                              \and 1 <= n2 <= domain_max_dim[1] 
 *                                              \and 1 <= n3 <= domain_max_dim[2]}
 * 
 * Therefore it iterates over all 'inner cells' over the Container, because the 
 * outermost layers of the cells correspond to the Halo Cells.
 * 
 */
class CellContainer::Iterator {
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
    Iterator(CellContainer& cell_cont) : cell(cell_cont) {};

    /**
     * @brief Constructor initializing dimensions and CellContainer reference.
     * @param cell_cont Reference to the CellContainer
     * @param x_ X cell dimension from which to start
     * @param y_ Y cell dimension from which to start
     * @param z_ Z cell dimension from which to start
     */
    Iterator(CellContainer& cell_cont, dim_t x_, dim_t y_, dim_t z_) : x(x_), y(y_), z(z_), cell(cell_cont) {};

    /**
     * @brief Prefix increment operator.
     * @return Reference to the incremented iterator.
     */
    Iterator& operator++();

    /**
     * @brief Dereference operator to obtain a vector of particles.
     * @return Reference to the vector of particles.
     */
    std::vector<Particle*>& operator*();

    /**
     * @brief Equality operator.
     * @param other Another Iterator to compare with.
     * @return True if both iterators are currently "pointing" to the same cell. (same x, y , z)
     */
    bool operator==(const Iterator& other);

    /**
     * @brief Inequality operator.
     * @param other Another Iterator to compare with.
     * @return True if iterators are not equal "pointing" to the same cell. (same x, y , z)
     */
    bool operator!=(const Iterator& other);



private:
    /**
     * @brief internal method to find next correct index
     * 
    */
    void next_correct_index(dim_t& x, dim_t& y, dim_t& z);
};


