/*
 * LinkedCellParticleContainer.h
 *
 * Created: 30.11.2022
 * Author:  marquardt
 */

#pragma once

#include "ParticleCell.h"
#include "ParticleContainer.h"

#include <vector>
#include <array>
#include <functional>

/**
 * @brief Particle Container that implements the linked cell algorithm. The plots below show how the algorithm can improve the performance of the simulation
 */
class LinkedCellParticleContainer : public ParticleContainer
{

private:
    std::vector<Particle> _activeParticleVector; // base vector to store all particles inside the domain

    std::vector<Particle> _haloParticleVector; // base vector to store all halo particles

    std::vector<ParticleCell> _cellVector; // stores all cells

    std::array<double, 3> _domain; // domain size in each dimension

    double _cutoff; // max. rdistance of particles where force calculation is applied

    std::array<int, 3> _numCells; // number of cells in each dimension

    std::array<double, 3> _cellSize; // cell size in each dimension

    double _reflectingDistance; // largest distance between particle and border, where reflecting boundary condition is applied: (6th root of 2) * sigma * 0.5

    /**
     * @brief compute index of cell the given particle belongs to
     * @param p particle
     * @return cell index
     */
    const int computeCellIdx(Particle &p);

    /**
     * @brief called if at least one particle crossed a cell boundary, update particle pointers in cells
     */
    const void updateCells();

    /**
     * @brief called if at least one particle crossed to a halo boundary, leads to restructuring of the cells
     */
    const void rebuildCells();

    /**
     * @brief computes number of cells and their size in each dimension, initializes them according to domain boundary conditions
     */
    const void initializeCells(std::array<BoundaryCondition, 6> &domainBoundaries);

public:
    LinkedCellParticleContainer(double sigma, double cutoff, std::array<double, 3> &domain, std::array<BoundaryCondition, 6> &boundaries);

    ~LinkedCellParticleContainer() override;

    /**
     * @brief applies given function to particle pairs within the cutoff radius considering each cell and its neighbors, also handles boundary conditions
     * @param f function which is applied to the particle pairs
     */
    const void iterateParticleInteractions(std::function<void(Particle &, Particle &)> f) override;

    /**
     * @brief applies given function to every particle, checks if they cross cell borders
     * @param f function which is applied to the particles
     */
    const void iterateParticles(std::function<void(Particle &)> f) override;

    /**
     * @brief adds particle to base vector and its pointer to the cell it belongs to
     * @param x The position array of the particle
     * @param v The velocity array of the particle
     * @param m The mass of the particle
     */
    const void addParticle(std::array<double, 3> &x, std::array<double, 3> &v, double &m, double &epsilon, double &sigma) override;

    /**
     * @brief Creates a new particle and adds it to the vector
     * @param x The position array of the particle
     * @param v The velocity array of the particle
     * @param m The mass of the particle
     * @param type The type of the particle
     * @param epsilon The epsilon of the particle
     * @param sigma The sigma of the particle
     */
    const void addParticle(std::array<double, 3> &x, std::array<double, 3> &v, double &m, double &epsilon, double &sigma, int &type) override;

    /**
     * @brief Creates a new particle and adds it to the vector
     * @param x The position array of the particle
     * @param v The velocity array of the particle
     * @param f The force acting on the particle
     * @param old_f The previous force acting on the particle
     * @param m The mass of the particle
     * @param type The type of the particle
     * @param epsilon The epsilon of the particle
     * @param sigma The sigma of the particle
     */
    const void addParticle(std::array<double, 3> &x, std::array<double, 3> &v, std::array<double, 3> &f, std::array<double, 3> &old_f, double &m, double &epsilon, double &sigma, int &type);

    /**
     * @brief adds reflection force to particles near to the reflecting boundary
     * @param particles particles inside the cell the reflecting boundary condition belongs to
     * @param boundary_idx position of reflecting boundary
     * @param f force calculation function which has to be applied at the reflecting boundary
     */
    const void reflectingBoundary(std::vector<Particle *> &particles, int boundary_idx, std::function<void(Particle &, Particle &)> f);

    const void initGhostParticles(std::vector<Particle *> &particles, int boundary_idx);

    /**
     * @brief removes halo particles from base vector, updates particle references in cells
     */
    const void clearHalo();

    /**
     * @brief Returns the number of particles in the simulation
     * @return The size of the particle vector
     */
    const int size() const override;

    /**
     * @brief deletes every particle (active & halo) from simulation, deletes particle pointers in cells
     */
    const void resetParticles() override;

    /**
     * @brief reserves additional memory for particles in basevector and additional memory in cells
     * @param numberOfParticles number of additional particles the space is needed for
     */
    const void reserveMemoryForParticles(int numberOfParticles) override;

    /**
     * @brief a function to get the BoundaryParticles
     * @returns the vector of boundary particles
     */
    std::vector<Particle> *getBoundaryParticles();

    std::array<double,3> mirroredPosition(Particle &p, int boundary_idx);

    std::vector<ParticleCell> &getCells();

    std::vector<Particle> &getHaloParticles();

    std::vector<Particle> &getActiveParticles() override;
};