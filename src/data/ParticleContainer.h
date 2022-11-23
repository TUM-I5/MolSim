#pragma once

#include "ParticleContainer.h"
#include "Particle.h"

#include <vector>
#include <array>
#include <string>
#include <iterator>
#include <functional>

/**
 * @brief wrapper class that stores and manages access to the particles
 *      The given implementation is a wrapper class around a std::vector<Particle>
 *      Another implementation might decide to use a different underlying structure.
 */
class ParticleContainer {
private:
    std::vector<double> force;
    std::vector<double> oldForce;
    std::vector<double> x;
    std::vector<double> v;
    std::vector<double> m;
    std::vector<int> type;
    unsigned long count;
    std::vector<std::vector<unsigned long>> cells;  // TODO @Johnny, ig this is fine for the cells

    /**
     * Stores a particle from @param p into the internal data at @param index
     * */
    void storeParticle(Particle &p, unsigned long index);

    /**
     * Loads a particle from the internal data into @param p at @param index
     * */
    void loadParticle(Particle &p, unsigned long index);

public:
    /**
     * @brief return the amount of particles stored
     *
     * @return int
     */
    unsigned long size();

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
    void runOnData(void (*fun)(std::vector<double> &force,
                               std::vector<double> &oldForce,
                               std::vector<double> &x,
                               std::vector<double> &v,
                               std::vector<double> &m,
                               std::vector<int> &type,
                               unsigned long count));

    /**
     * @brief Applies the given function to all Particles
     *
     * @param function
     */
    void forAllParticles(void (*function)(Particle &));

    /**
     * @brief Applies the given function to all Particles
     *
     * @param function
     */
    void forAllParticles(std::function<void(Particle&)>);

    /**
     * @brief Applies given function to all pairs of Particles p_i, p_j, where p_i < p_j once
     *  (If f(p_i, p_j) got invoked, f(p_j, p_i) won't get invoked with the same i and j)
     * @param function
     */
    void forAllPairs(void (function)(Particle &p1, Particle &p2));

    /**
     * Removes all particles.
     * */
    void clear();

    /**
     * Get a copy of particle at position @param i
     * */
    Particle getParticle(unsigned long i);
};