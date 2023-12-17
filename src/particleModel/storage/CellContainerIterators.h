#pragma once

#include "CellContainer.h"

typedef int dim_t;


class CellContainer::CustomIterator{
public:
    /** the first cell is on the left lower corner of a cuboid/ rectangle domain*/
    dim_t lower_x = 1; 
    dim_t upper_x;
    dim_t lower_y = 1; 
    dim_t upper_y; 
    dim_t lower_z = 1; 
    dim_t upper_z;

    dim_t x = 1; 
    dim_t y = 1; 
    dim_t z = 1; 

    CellContainer& cell; /** the Container over which boundary cells the iterator iterates*/

    /**
     * @brief Constructor initializing dimensions and CellContainer reference.
     * @param cell_cont Reference to the CellContainer
     */
    CustomIterator(CellContainer& cell_cont) : cell(cell_cont) {};


    /**
     * @brief Constructor initializing dimensions and CellContainer reference.
     * @param cell_cont Reference to the CellContainer
     * @param x_ X cell dimension from which to start
     * @param y_ Y cell dimension from which to start
     * @param z_ Z cell dimension from which to start
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
     * @param other Another Iterator to compare with.
     * @return True if both iterators are currently "pointing" to the same cell. (same x, y , z)
     */
    bool operator==(const CustomIterator& other);

    /**
     * @brief Inequality operator.
     * @param other Another Iterator to compare with.
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


