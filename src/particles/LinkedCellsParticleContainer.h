#pragma once

#include <iterator>
#include <vector>

#include "particles/Cell.h"
#include "particles/ParticleContainerInterface.h"

/**
 * @brief Extension of the `ParticleContainer` class using a linked cells data structure for improved performance
 */
class LinkedCellsParticleContainer : public ParticleContainerInterface {
   private:
    std::vector<Particle> particles;

    std::array<double, 3> domain_size;
    double cutoff_radius;

    std::array<double, 3> cell_size;
    std::array<int, 3> domain_num_cells;

    std::vector<Cell> cells;

    std::vector<Cell*> inner_cell_references;
    std::vector<Cell*> boundary_cell_references;
    std::vector<Cell*> halo_cell_references;

   public:
    struct BoundaryIterator {
       public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Particle;
        using difference_type = std::ptrdiff_t;
        using pointer = Particle*;
        using reference = Particle&;

       private:
        std::vector<Cell*>& cells;
        int cell_index = -1;
        int particle_index = -1;

       public:
        BoundaryIterator(std::vector<Cell*>& cells, int cell_index, int particle_index);

        BoundaryIterator& operator++();

        Particle& operator*() const;
        Particle* operator->() const;

        bool operator==(const BoundaryIterator& other) const;
        bool operator!=(const BoundaryIterator& other) const;
    };

    /**
     * @brief Returns an iterator to the first boundary particle
     * @return Iterator to the first boundary particle
     *
     * Returns an iterator to the first boundary particle.
     */
    BoundaryIterator boundaryBegin();

    /**
     * @brief Returns an iterator to the last boundary particle
     * @return Iterator to the last boundary particle
     *
     * Returns an iterator to the last boundary particle.
     */
    BoundaryIterator boundaryEnd();

    /**
     * @brief Construct a new Linked Cells Particle Container object
     *
     * @param size the size of the domain
     * @param cutoff_radius the cutoff radius for the force calculation
     * @param n the expected number of particles (for preallocation of memory)
     *
     * Constructs a new Linked Cells Particle Container object using the specified domain size and cutoff radius.
     * The expected number of particles is used to preallocate memory for the particle vector.
     * The origin (left lower front corner of the boundary) is assumed to be at (0, 0, 0).
     * The cutoff radius is used to determine the number of cells in each dimension, where the cell size is bigger or equal then the cutoff
     * radius but adjusted to divide the domain size evenly into a whole number of cells. Ideally the domain size is a multiple of the
     * cutoff and cube shaped. Internally, the domain is extended by one cell in each direction to allow for so called 'ghost' or 'halo'
     * cells that are used for boundary condition handling. Therefore the valid cell coordinates range from -1 to domain_num_cells[i] in
     * each dimension (i = 0 -> x; i = 1 -> y; i = 2 -> z).
     */
    LinkedCellsParticleContainer(const std::array<double, 3>& domain_size, double cutoff_radius, int n = 0);

    /**
     * @brief Adds a particle to the container
     * @param p Particle to be added
     *
     * Adds a particle to the container and correctly inserts it into the cell structure.
     */
    void addParticle(const Particle& p) override;

    /**
     * @brief Adds a particle to the container
     * @param p Particle to be added
     *
     * Adds a particle to the container and correctly inserts it into the cell structure.
     */
    void addParticle(Particle&& p) override;

    /**
     * @brief Applies the given force sources to the particles
     * @param force_sources List of force sources to be applied
     *
     * Applies the given force sources to the particles in the container.
     * Uses newton's third law to calculate the forces between the particles in an optimized way.
     * Additionally to the functionality of the `ParticleContainer` class, this method uses the internal cell structure to
     * reduce the number of force calculations necessary, depending on the cutoff radius.
     */
    void applyPairwiseForces(const std::vector<std::unique_ptr<ForceSource>>& force_sources) override;

    /**
     * @brief Returns the number of particles in the container
     * @return Number of particles in the container
     */
    size_t getNumParticles() const { return particles.size(); }

    /**
     * @brief Returns the domain size
     * @return Domain size
     *
     * Returns the domain size as a 3D array.
     */
    const std::array<double, 3>& getDomainSize() const { return domain_size; }

    /**
     * @brief Returns the cutoff radius
     * @return Cutoff radius
     *
     * Returns the cutoff radius used for the force calculation.
     */
    double getCutoffRadius() const { return cutoff_radius; }

    /**
     * @brief Returns the cells
     * @return Cells
     *
     * Returns the cells as a vector of `Cell` objects.
     */
    const std::vector<Cell>& getCells() const { return cells; }

    /**
     * @brief Returns the pointers of the boundary cells
     * @return Vector of pointers to the boundary cells
     *
     * Returns the pointers of the boundary cells in a vector.
     */
    const std::vector<Cell*>& getBoundaryCells() const { return boundary_cell_references; }

    /**
     * @brief Returns the cell size
     * @return Cell size
     *
     * Returns the cell size as a 3D array.
     */
    const std::array<double, 3>& getCellSize() const { return cell_size; }

    /**
     * @brief Returns the number of cells in each dimension
     * @return Number of cells in each dimension
     *
     * Returns the number of cells in each dimension as a 3D array.
     */
    const std::array<int, 3>& getDomainNumCells() const { return domain_num_cells; }

    /**
     * @brief Maps the cell coordinates to the corresponding index in the internal cell vector
     *
     * @param cx x coordinate of the cell (valid range: -1 to domain_num_cells[0])
     * @param cy y coordinate of the cell (valid range: -1 to domain_num_cells[1])
     * @param cz z coordinate of the cell (valid range: -1 to domain_num_cells[2])
     * @return index of the cell if it exists, -1 otherwise
     */
    int cellCoordToCellIndex(int cx, int cy, int cz) const;

    /**
     * @brief Maps the particle position to the corresponding cell index in the internal cell vector
     *
     * @param pos Position of the particle
     * @return index of the cell if it exists, -1 otherwise
     */
    Cell* particlePosToCell(const std::array<double, 3>& pos);

    /**
     * @brief Maps the particle position to the corresponding cell index in the internal cell vector
     *
     * @param pos Position of the particle
     * @return index of the cell if it exists, -1 otherwise
     */
    Cell* particlePosToCell(double x, double y, double z);
};