//
// Created by berks on 17.11.2023.
//

#pragma once

#include <vector>
#include <array>
#include <list>
#include <set>
#include "Particle.h"
#include "ParticleContainer.h"
#include "BoundaryBehavior.h"

/**
 * A container that stores particles in a linked cell data structure. The cells are stored in a 1D vector.
 *
 * This implementation is significantly faster than the naive implementation
 * @image html plot.png width=500px
 */
class LinkedCellParticleContainer : public ParticleContainer{
private:
    double xSize;
    double ySize;
    double zSize;

    int xCells;
    int yCells;
    int zCells;

    double  cutoffRadius;

    double cellXSize;
    double cellYSize;
    double cellZSize;

    double deltaT;
    /**
     * The vector that contains all the particles in the container
     */

    std::vector<std::vector<Particle>> cells;

    std::set<int> boundaryCellIndices;

    std::set<int> haloCellIndices;

    std::vector<bool> isHaloCellVector;

    BoundaryBehavior boundaryBehaviorTop;
    BoundaryBehavior boundaryBehaviorBottom;
    BoundaryBehavior boundaryBehaviorRight;
    BoundaryBehavior boundaryBehaviorLeft;
    BoundaryBehavior boundaryBehaviorFront;
    BoundaryBehavior boundaryBehaviorBack;

public:

    /**
     * @brief Construct a new Linked Cell Particle Container object
     * @note Delegates to the other constructor with all boundary behaviors set to 'Reflective'
     * @param xSize
     * @param ySize
     * @param zSize
     * @param cutoffRadius
     * @param deltaT
     */
    LinkedCellParticleContainer(double xSize, double ySize, double zSize, double cutoffRadius, double deltaT);

    /**
     * @brief Construct a new Linked Cell Particle Container object
     * @param xSize
     * @param ySize
     * @param zSize
     * @param cutoffRadius
     * @param deltaT
     * @param boundaryBehaviorTop
     * @param boundaryBehaviorBottom
     * @param boundaryBehaviorRight
     * @param boundaryBehaviorLeft
     * @param boundaryBehaviorFront
     * @param boundaryBehaviorBack
     */
    LinkedCellParticleContainer(double xSize, double ySize, double zSize, double cutoffRadius, double deltaT, BoundaryBehavior boundaryBehaviorTop, BoundaryBehavior boundaryBehaviorBottom, BoundaryBehavior boundaryBehaviorRight, BoundaryBehavior boundaryBehaviorLeft, BoundaryBehavior boundaryBehaviorFront, BoundaryBehavior boundaryBehaviorBack);

    /**
     * @brief Construct a new Linked Cell Particle Container object
     * @note Delegates to the other constructor with all boundary behaviors set to the given value
     * @param xSize
     * @param ySize
     * @param zSize
     * @param cutoffRadius
     * @param deltaT
     * @param boundaryBehaviorAll
     */
    LinkedCellParticleContainer(double xSize, double ySize, double zSize, double cutoffRadius, double deltaT, BoundaryBehavior boundaryBehaviorAll);

    ~LinkedCellParticleContainer();

    /**
     * @brief Get the index of a cell with given coordinates (3d index)
     *
     * @param x
     * @param y
     * @param z
     * @return 1d index
     */
    int index3dTo1d(int x, int y, int z);

    /**
     * @brief Get the coordinates (3d index) of a cell with given index (1d)
     *
     * @param index
     * @return Coordinates
     */
    std::array<int, 3> index1dTo3d(int index);

    /**
     * @brief Apply a lambda function to all particle pairs
     * Checks the neighbor cells
     * Each combination is iterated only once (e.g. useful for Newton's 3rd law)
     * @param function
     */
    virtual void applyToAllPairsOnce(const std::function<void(Particle &, Particle &)> &function);

    /**
     * @brief Apply a lambda function to all particles
     * @param function
     */
    virtual void applyToAll(const std::function<void(Particle &)> &function);

    /**
     * @brief Find the correct cell for a given particle
     * @param particle
     *
     * @return 1d index of the cell the given particle belongs to
     */
    int cellIndexForParticle(const Particle &particle);

    /**
     * @brief Add a particle to the container
     * @note The particle is added to the correct cell (using cellIndexForParticle)
     * @param particle
     */
    void add(const Particle &particle);

    /**
     * @brief Update particles in the cell adressed by the given index
     * Remove the particles that are not in the cell anymore, add them to new cells if necessary
     * @param cellIndex
     *
     * @param cellIndex
     */
    void updateParticleCell(int cellIndex);

    /**
     * @brief Apply a lambda function to all particles
     * @note updateParticleCell is called if updateCells is true
     *
     * @param function
     * @param updateCells
     */
    void applyToAll(const std::function<void(Particle &)> &function, bool updateCells);

    /**
     * @brief Add a particle to the cell at the given index
     *
     * @param cellIndex
     * @param particle
     */
    void addParticleToCell(int cellIndex, const Particle &particle);

    /**
     * @brief Return number of particles in the container
     *
     * @return Number of particles
     */
    int size();

    /**
     * @brief Return an updated position vector after reflecting on the given axis
     *
     * @param position
     * @param axisIndex
     * @param boundary
     * @return Reflected position
     */
    static std::array<double, 3> updatePositionOnReflection(const std::array<double, 3> &position, int axisIndex, double boundary);

    /**
     * @brief Return an updated velocity vector after reflecting on the given axis
     *
     * @param velocity
     * @param axisIndex
     * @return Reflected velocity
     */
    static std::array<double, 3> updateVelocityOnReflection(std::array<double, 3> velocity, int axisIndex);

    /**
     * @brief Remove all particles from halo cells.
     * Clear halo cells.
     */
    void deleteParticlesInHaloCells();

    /**
     * @brief Check and reflect a particle
     * @note All boundaries are checked. Delegates to reflectIfNecessaryOnAxis.
     *
     * @param cellIndex
     * @return Cell
     */
    void vectorReverseReflection(Particle &particle);

    /**
     * @brief Check and reflect a particle on the given axis
     *
     * @param particle
     * @param axisMin
     * @param axisMax
     * @param axisIndex
     * @return true if reflected, else false
     */
    bool reflectIfNecessaryOnAxis(Particle &particle, double axisMin, double axisMax, int axisIndex);

    nlohmann::ordered_json json();

    std::string toString();

    double getXSize() const;

    double getYSize() const;

    double getZSize() const;

    int getXCells() const;

    int getYCells() const;

    int getZCells() const;

    double getCutoffRadius() const;

    double getCellXSize() const;

    double getCellYSize() const;

    double getCellZSize() const;

    double getDeltaT() const;

    const std::vector<std::vector<Particle>> &getCells() const;

    const std::set<int> &getBoundaryCellIndices() const;

    const std::set<int> &getHaloCellIndices() const;

    const std::vector<bool> &getIsHaloCellVector() const;

    BoundaryBehavior getBoundaryBehaviorTop() const;

    BoundaryBehavior getBoundaryBehaviorBottom() const;

    BoundaryBehavior getBoundaryBehaviorRight() const;

    BoundaryBehavior getBoundaryBehaviorLeft() const;

    BoundaryBehavior getBoundaryBehaviorFront() const;

    BoundaryBehavior getBoundaryBehaviorBack() const;
};


