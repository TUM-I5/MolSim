#pragma once

#include "ParticleContainer.h"
#include "Particle.h"
#include "sim/physics/bounds/types.h"

#include <vector>
#include <array>
#include <string>
#include <iterator>
#include <functional>
#include <unordered_set>

/**
 * @brief wrapper class that stores and manages access to the particles
 *      The given implementation is a wrapper class around a std::vector<Particle>
 *      Another implementation might decide to use a different underlying structure.
 */
class ParticleContainer {
private:
    double root6_of_2;
    std::vector<double> force;
    std::vector<double> oldForce;
    std::vector<double> x;
    std::vector<double> v;
    std::vector<double> m;
    std::vector<int> type;
    unsigned long count;
    std::vector<unsigned long> activeParticles;
    std::vector<std::vector<unsigned long>> cells;
    std::array<unsigned int, 3> gridDimensions; //stores the number of cells in x- y- and z- direction
    std::array<double, 3> domainSize;
    double x_2_max;
    double x_1_max;
    double x_0_max;
    double x_2_min;
    double x_1_min;
    double x_0_min;
    double r_cutoff;

    /**
     * Stores a particle from @param p into the internal data at @param index
     * */
    void storeParticle(Particle &p, unsigned long index);

    /**
     * Stores a particle from @param p into the provided buffers.
     * */
    static void storeParticle(Particle &p, unsigned long index, std::vector<double>& force, std::vector<double>& oldForce, std::vector<double>& x, std::vector<double>& v, std::vector<double>& m, std::vector<int>& type);

    /**
     * Loads a particle from the internal data into @param p at @param index
     * */
    void loadParticle(Particle &p, unsigned long index);

    /**
     * Loads a particle from the provided buffer into @param p at @param index
     * */
    static void loadParticle(Particle &p, unsigned long index, std::vector<double>& force, std::vector<double>& oldForce, std::vector<double>& x, std::vector<double>& v, std::vector<double>& m, std::vector<int>& type);

public:
    /**
     * @brief Construct a new Particle Container object with no particles stored
     *
     */
    ParticleContainer();

    /**
     * @brief Construct a new Particle Container that stores the given particles
     *
     * @param buffer for all particles. will be added to local storage.
     */
    explicit ParticleContainer(const std::vector<Particle> &buffer);

    /**
     * @brief Constructor of ParticleContainer that also initializes the cell-structure
     *
     * @param buffer
     * @param domainSize
     * @param r_cutoff
     */
    ParticleContainer(const std::vector<Particle>& buffer, std::array<double, 3> domainSize, double r_cutoff);

    /**
     * @brief Constructor of ParticleContainer that also initializes a seemingly two dimensional cell-structure
     *
     * @param buffer
     * @param domainSize
     * @param r_cutoff
     */
    ParticleContainer(const std::vector<Particle>& buffer, std::array<double, 2> domainSize, double r_cutoff);

    /**
     * @brief returns the index of the cell in cells corresponding to the coordinates given. Performs NO bounds checks!
     * Example: cellIndexFromCellCoordinates({0,0,0})->0
     * because the cell at position {0,0,0} is stored at index 0 in cells
     * @param coords
     * @return int
     */
    unsigned int cellIndexFromCellCoordinatesFast(unsigned int x0, unsigned int x1, unsigned int x2);

    /**
     * @brief returns the index of the cell in cells corresponding to the coordinates given. Performs bounds checks
     * Example: cellIndexFromCellCoordinates({0,0,0})->0
     * because the cell at position {0,0,0} is stored at index 0 in cells
     * @param coords
     * @return int
     */
    unsigned int cellIndexFromCellCoordinates(std::array<unsigned int, 3> coords);

    /**
     * @brief return the amount of particles stored
     *
     * @return int
     */
    unsigned long size();

    /**
     * @brief Returns the amount of the active particles
     * */
     unsigned long activeSize();

    /**
     * Makes sure that every Particle (or every index corresponding to the Particle) is in the
s    * right corresponding cell-vector
     */
    void updateCells();

    /**
     * Removes all particles.
     * */
    void clear();

    /**
     * Get a copy of particle at position @param i
     * */
    Particle getParticle(unsigned long i);

    /**
     * @brief Generates all halo particles, that are not stored yet
     * */
    template<sim::physics::bounds::side S>
    void forEachParticleHaloPairInSide(const double sigma, const std::function<void(Particle &, Particle &)>& function) {
        double maxBorderDistance = root6_of_2 * sigma / 2;
        if constexpr (S == sim::physics::bounds::side::left) {
            // left x = x_0 = min
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto& cell_indices_left = cells[cellIndexFromCellCoordinates({0, x_1, x_2})];
                    for (auto i : cell_indices_left) {
                        if (x[3 * i + 0] != 0 && x[3 * i + 0] < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            auto& pos = p_real.getX();
                            Particle p_halo = p_real;
                            p_halo.setX({(-1)*pos[0],pos[1],pos[2]});

                            function(p_real, p_halo);
                            storeParticle(p_real, i);
                        }
                    }
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::right) {
            // right x = x_0 = max and left x = x_0 = min
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto& cell_indices_right = cells[cellIndexFromCellCoordinates({gridDimensions[0] - 1, x_1, x_2})];
                    for (auto i : cell_indices_right) {
                        double distance = domainSize[0] - x[3 * i + 0];
                        if (distance < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            Particle p_halo = p_real;
                            p_halo.add_to_X({distance*2,0,0});

                            function(p_real, p_halo);
                            storeParticle(p_real, i);
                        }
                    }
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::bottom) {
            // top y = x_1 = max and bottom y = x_1 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto& cell_indices_bot = cells[cellIndexFromCellCoordinates({x_0, 0, x_2})];
                    for (auto i : cell_indices_bot) {
                        if (x[3 * i + 1] != 0 && x[3 * i + 1] < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            auto& pos = p_real.getX();
                            Particle p_halo = p_real;
                            p_halo.setX({pos[0],(-1)*pos[1],pos[2]});

                            function(p_real, p_halo);
                            storeParticle(p_real, i);
                        }
                    }
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::top) {
            // top y = x_1 = max and bottom y = x_1 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto& cell_indices_top = cells[cellIndexFromCellCoordinates({x_0, gridDimensions[1] - 1, x_2})];
                    for (auto i : cell_indices_top) {
                        double distance = domainSize[1] - x[3 * i + 1];
                        if (distance < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            Particle p_halo = p_real;
                            p_halo.add_to_X({0, distance*2,0});

                            function(p_real, p_halo);
                            storeParticle(p_real, i);
                        }
                    }
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::front) {
            // back z = x_2 = max and front z = x_2 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                    auto& cell_indices_front = cells[cellIndexFromCellCoordinates({x_0, x_1, 0})];
                    for (auto i : cell_indices_front) {
                        if (x[3 * i + 2] != 0 && x[3 * i + 2] < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            auto& pos = p_real.getX();
                            Particle p_halo = p_real;
                            p_halo.setX({pos[0],pos[1],(-1)*pos[2]});

                            function(p_real, p_halo);
                            storeParticle(p_real, i);
                        }
                    }
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::rear) {
            // back z = x_2 = max and front z = x_2 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                    auto& cell_indices_back = cells[cellIndexFromCellCoordinates({x_0, x_1, gridDimensions[2] - 1})];
                    for (auto i : cell_indices_back) {
                        double distance = domainSize[2] - x[3 * i + 2];
                        if (distance < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            Particle p_halo = p_real;
                            p_halo.add_to_X({0,0,distance*2});

                            function(p_real, p_halo);
                            storeParticle(p_real, i);
                        }
                    }
                }
            }
        }
    }

    /**
     * Get the indices of all particles, that are outside of the domain.
     * Will only use the correct branch. This is determined at compile time.
     * */
    template<sim::physics::bounds::side S>
    void getExternalParticles(std::unordered_set<unsigned long> &output) {
        if constexpr (S == sim::physics::bounds::side::left) {
            // right x = x_0 = max and left x = x_0 = min
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto& cell_indices_left = cells[cellIndexFromCellCoordinates({0, x_1, x_2})];
                    for (auto i : cell_indices_left) if (x[3 * i + 0] < x_0_min) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::right) {
            // right x = x_0 = max and left x = x_0 = min
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto& cell_indices_right = cells[cellIndexFromCellCoordinates({gridDimensions[0] - 1, x_1, x_2})];
                    for (auto i : cell_indices_right) if (x[3 * i + 0] > x_0_max) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::bottom) {
            // top y = x_1 = max and bottom y = x_1 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto& cell_indices_bot = cells[cellIndexFromCellCoordinates({x_0, 0, x_2})];
                    for (auto i : cell_indices_bot) if (x[3 * i + 1] < x_1_min) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::top) {
            // top y = x_1 = max and bottom y = x_1 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto& cell_indices_top = cells[cellIndexFromCellCoordinates({x_0, gridDimensions[1] - 1, x_2})];
                    for (auto i : cell_indices_top) if (x[3 * i + 1] > x_1_max) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::front) {
            // back z = x_2 = max and front z = x_2 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                    auto& cell_indices_front = cells[cellIndexFromCellCoordinates({x_0, x_1, 0})];
                    for (auto i : cell_indices_front) if (x[3 * i + 2] < x_2_min) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::rear) {
            // back z = x_2 = max and front z = x_2 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                    auto& cell_indices_back = cells[cellIndexFromCellCoordinates({x_0, x_1, gridDimensions[2] - 1})];
                    for (auto i : cell_indices_back) if (x[3 * i + 2] > x_2_max) output.emplace(i);
                }
            }
        }
    }

    /**
     * Removes the specified indices from the active list. For this to have effect updateCells has to be called.
     * */
    void deactivateParticles(std::unordered_set<unsigned long>& indices);

    /**
     * @brief getter for gridDimensions.
     * There are gridDimensions[0]*gridDimensions[1]*gridDimensions[2] cells used
     *
     */
    std::array<unsigned int, 3> getGridDimensions();

    /**
     * Performs fun once. Provides all internal data to the lambda.
     * */
    void runOnData(void (*fun)(std::vector<double> &force,
                               std::vector<double> &oldForce,
                               std::vector<double> &x,
                               std::vector<double> &v,
                               std::vector<double> &m,
                               std::vector<int> &type,
                               unsigned long count,
                               std::vector<std::vector<unsigned long>>& cells));

    /**
     * Runs the function on the internal data
     * */
    void runOnData(void(*function)(std::vector<double> &force,
                                   std::vector<double> &oldForce,
                                   std::vector<double> &x,
                                   std::vector<double> &v,
                                   std::vector<double> &m,
                                   std::vector<int> &type,
                                   unsigned long count));

    /**
    * Runs the function on the internal data
    * */
    void runOnData(const std::function<void(std::vector<double> &force,
                                      std::vector<double> &oldForce,
                                      std::vector<double> &x,
                                      std::vector<double> &v,
                                      std::vector<double> &m,
                                      std::vector<int> &type,
                                      unsigned long count)>& function);

    /**
     * @brief Applies the given function to all Particles
     *
     * @param function
     */
    void forAllParticles(const std::function<void(Particle&)>& function);

    /**
     * @brief Applies the given function to all Particles
     *
     * @param function
     */
    void forAllParticles(void(*function)(Particle&));

    /**
     * @brief Applies given function to all pairs of Particles p_i, p_j, where p_i < p_j once
     *  (If f(p_i, p_j) got invoked, f(p_j, p_i) won't get invoked with the same i and j)
     * @param function
     */
    void forAllPairs(void (*function)(Particle &p1, Particle &p2));

    /**
     * @brief Applies given function to all pairs of Particles p_i, p_j, where p_i < p_j once
     *  (If f(p_i, p_j) got invoked, f(p_j, p_i) won't get invoked with the same i and j)
     * @param function
     */
    void forAllPairs(const std::function<void(Particle &p1, Particle &p2)>& function);

    /**
     * Performs fun on provided data. All lambda args particle container internal data.
     * Will be applied on every cell.
     * */
    void forAllCells(void (*fun)(std::vector<double> &force,
                                 std::vector<double> &oldForce,
                                 std::vector<double> &x,
                                 std::vector<double> &v,
                                 std::vector<double> &m,
                                 std::vector<int> &type,
                                 unsigned long count,
                                 std::vector<unsigned long>& cellItems));

    /**
     * Performs fun on provided data. All lambda args particle container internal data.
     * Will be applied on every cell.
     * */
    void forAllCells(std::function<void(std::vector<double> &force,
                                        std::vector<double> &oldForce,
                                        std::vector<double> &x,
                                        std::vector<double> &v,
                                        std::vector<double> &m,
                                        std::vector<int> &type,
                                        unsigned long count,
                                        std::vector<unsigned long>& cellItems)> fun);

    /**
     * @brief Performs function on all Pairs that are in the same cell
     * 
     * @param function 
     */
    void forAllPairsInSameCell(const std::function<void(Particle &p1, Particle &p2)>& function);

    /**
     * @brief Performs function on all Pairs that are in neighbouring cells. Does not perform fun if 2 particles are in the same cell
     * 
     * @param function 
     */
    void forAllPairsInNeighbouringCell(const std::function<void(Particle &p1, Particle &p2)>& function);

    /**
     * Performs fun on provided data. All lambda args particle container internal data.
     * Will be applied on every distinct cell pair. (Set-Wise) I.e. {a,b} = {b,a}.
     * */
    void forAllDistinctCellPairs(void (*fun)(std::vector<double> &force,
                                             std::vector<double> &oldForce,
                                             std::vector<double> &x,
                                             std::vector<double> &v,
                                             std::vector<double> &m,
                                             std::vector<int> &type,
                                             unsigned long count,
                                             std::vector<unsigned long>& cell0Items,
                                             std::vector<unsigned long>& cell1Items));


    /**
     * Performs fun on provided data. All lambda args particle container internal data.
     * Will be applied on every distinct cell neighbours. (Set-Wise) I.e. {a,b} = {b,a}.
     * */
    void forAllDistinctCellNeighbours(std::function<void(std::vector<double> &force,
                                                         std::vector<double> &oldForce,
                                                         std::vector<double> &x,
                                                         std::vector<double> &v,
                                                         std::vector<double> &m,
                                                         std::vector<int> &type,
                                                         unsigned long count,
                                                         std::vector<unsigned long>& cell0Items,
                                                         std::vector<unsigned long>& cell1Items)> fun);

};