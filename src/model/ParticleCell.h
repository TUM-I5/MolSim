/*
 * ParticleCell.h
 *
 * Created: 02.12.2022
 * Author:  marquardt
 */

#pragma once

#include "Particle.h"

#include <vector>

/**
 * @brief Enum for different types of cells
*/
enum class CellType {
    InnerCell,
    BoundaryCell
};

/**
 * @brief Enum for different types of boundary conditions
*/
enum class BoundaryCondition {
    Outflow, 
    Reflecting,
    None //maybe not needed
};

/**
 * @brief class for cells used in the linked cell algorithm
*/
class ParticleCell {
    private:

        std::vector<Particle *> _particles; //vector of pointers to particles currently in this cell

        std::vector<int> _neighbours; //structure to store index of neighboring cells with a higher index
        
        CellType _type; //type of cell (inner or boundary)

        int _invalidCount; //number of invalid particle pointers in cell
        
        /**
         * array to store boundary conditions for each cell
         * first two indices for x-direction (left, right)
         * middle two indices for y-direction (top, bottom)
         * last two indices for z-direction (front, back)
        */
        std::array<BoundaryCondition, 6> _boundaries;  

        std::shared_ptr<spdlog::logger> _memoryLogger; //a speedlog logger which logs construction and destruction of particles
    
    public:

        /**
         * @brief constructor for the particle cell, initializes loggers and variables
         * @param type type of cell (inner, boundary)
         * @param boundaries boundary condition on each border of the cell
        */
        ParticleCell(CellType type, std::array<BoundaryCondition, 6> boundaries);
        ~ParticleCell();

        /**
         * @brief replaces first invalid particle with given pointer
         * @param p pointer to new particle
        */
        const void insertParticle(Particle *p);

        /**
         * @brief computes given function for every particle pair inside the cell (making use of Newton's third law)
         * @param f function to be applied to particle pairs
        */
        const void iterateParticlePairs(std::function<void(Particle &, Particle &)> f);

        /**
         * @brief removes all particles from cell
        */
        const void clearCell();

        /**
         * @brief reserves extra vector space for given number of particles
         * @param meanParticles mean number of particles per cell, estimate for actual number of particles
        */
        const void reserveMemory(int meanParticles);

        /**
         * @brief updates count of invalid particle pointers inside cell
        */
        const void updateInvalidCounter();

        /**
         * @brief removes invalid pointers and returns sanitized particle pointers
        */
        std::vector<Particle *> &getCellParticles();

        const std::array<BoundaryCondition, 6> &getBoundaries();

        const CellType getType();

        const int size();

        const std::string toString();

        const std::vector<int> &getNeighbours();

        const void setNeighbours(std::vector<int> &neighbours);

        
};