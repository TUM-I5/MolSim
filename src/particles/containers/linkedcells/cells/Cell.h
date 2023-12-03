#pragma once

#include <array>
#include <optional>
#include <unordered_set>
#include <vector>

#include "particles/Particle.h"

/**
 * @brief Class representing a cell in the linked cells algorithm
 */
class Cell {
   public:
    /**
     * @brief Enum representing the type of a cell
     */
    enum class CellType { INNER, BOUNDARY, HALO };

   private:
    /**
     * @brief Type of the cell
     */
    CellType cell_type;

    /**
     * @brief References to the particles the cell contains
     */
    std::vector<Particle*> particle_references;

    /**
     * @brief References to the neighbouring cells
     */
    std::vector<Cell*> neighbour_references;

    /**
     * @brief References to the cells that have already influenced this cell
     */
    std::unordered_set<Cell*> already_influenced_by;

   public:
    /**
     * @brief Construct a new Cell object
     *
     * @param cell_type Type of the cell
     */
    explicit Cell(CellType cell_type);

    /**
     * @brief Get the type of the cell
     *
     * @return Type of the cell
     */
    CellType getCellType() const;

    /**
     * @brief Get the reference vector for the particles the cell contains
     *
     * @return Vector of references to the particles the cell contains
     */
    std::vector<Particle*>& getParticleReferences();

    /**
     * @brief Get the reference vector for the neighbouring cells
     *
     * @return Vector of references to the neighbouring cells
     */
    std::vector<Cell*>& getNeighbourReferences();

    /**
     * @brief Get the reference set for the cells that have already influenced this cell during force calculation
     *
     * @return Set of references to the cells that have already influenced this cell
     */
    std::unordered_set<Cell*>& getAlreadyInfluencedBy();

    /**
     * @brief Adds a particle reference to the cell
     *
     * @param p Particle reference to be added
     */
    void addParticleReference(Particle* p);

    /**
     * @brief Removes all particle references from the cell
     */
    void clearParticleReferences();

    /**
     * @brief Adds a neighbour reference to the cell
     *
     * @param c Cell reference to be added
     */
    void addNeighbourReference(Cell* c);

    /**
     * @brief Adds a cell reference to the set of cells that have already influenced this cell
     *
     * @param c Cell reference to be added
     */
    void addAlreadyInfluencedBy(Cell* c);

    /**
     * @brief Removes all cell references from the set of cells that have already influenced this cell
     */
    void clearAlreadyInfluencedBy();
};