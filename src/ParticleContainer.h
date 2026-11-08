#pragma once
#include <type_traits>
#include <vector>

#include "utils/Vec3.h"

/**
 * @brief Promises the compiler that iterations in the loop are independent from each other
 *
 * Place it directly before a loop, it allows vectorizing loops the compiler cant prove independent
 * iterations for.
 *
 * Also allows reordering of floating point sums inside the loop when using clang.
 */
#if defined(__clang__)
#define INDEPENDENT_ITERATIONS _Pragma("clang loop vectorize(assume_safety)")
#elif defined(__GNUC__)
#define INDEPENDENT_ITERATIONS _Pragma("GCC ivdep")
#else
#define INDEPENDENT_ITERATIONS
#endif

/**
 * @brief Handle to a Single Particle in the Container
 *
 * Stores only the index of the particle because the entries are not consecutive in memory because
 * of SoA layout.
 *
 * Because its only the index and reference to the container its cheap to copy.
 *
 * @tparam Container `ParticleContainer` for read / write, `const ParticleContainer` for read only
 */
template <class Container>
class ParticleRef {
    std::reference_wrapper<Container>
        container_;      ///< Container that stores the data of the particle
    std::size_t index_;  ///< index of the Particle in the Container

   public:
    /**
     * @brief Creates a handle to the particle at index @p index.
     * @param container Container that holds the Particle
     * @param index Index of the Particle in the Container
     */
    ParticleRef(Container &container, const std::size_t index)
        : container_(container), index_(index) {}

    /// @name Read access
    /// @{
    Vec3 x() const { return container_.get().x(index_); }          ///< Position
    Vec3 v() const { return container_.get().v(index_); }          ///< Velocity
    Vec3 f() const { return container_.get().f(index_); }          ///< Force
    Vec3 old_f() const { return container_.get().old_f(index_); }  ///< Force of the previous step
    double m() const { return container_.get().m(index_); }        ///< Mass
    int type() const { return container_.get().type(index_); }     ///< Type of Particle
    std::size_t index() const { return index_; }  ///< Index in the Container that stores it
    /// @}

    /// @name Write access (For non const Container)
    /// @{
    void SetX(const Vec3 &x) { container_.get().SetX(index_, x); }  ///< Sets Position
    void SetV(const Vec3 &v) { container_.get().SetV(index_, v); }  ///< Sets Velocity
    void SetF(const Vec3 &f) { container_.get().SetF(index_, f); }  ///< Sets Current Force
    void SetOldF(const Vec3 &old_f) {
        container_.get().SetOldF(index_, old_f);
    }  ///< Sets old Force
    void SetM(const double m) { container_.get().SetM(index_, m); }           ///< Sets Mass
    void SetType(const int type) { container_.get().SetType(index_, type); }  ///< Set Type tag
    /// @}
};

/**
 * @brief Forward iterator over all particles of a container
 *
 * Dereferencing yields a `ParticleRef` by value (particle objects are never created so cant
 * be pointed to)
 *
 * @tparam Container ParticleContainer or `const ParticleContainer` for read only iteration
 */
template <class Container>
class ParticleIterator {
    std::reference_wrapper<Container> container_;  ///< Container iterating over
    std::size_t index_;                            ///< current index

   public:
    /**
     * @brief Creates a forward iterator pointing to Particle at index @p index in container
     * @p container
     *
     * @param container Container to iterate over
     * @param index Index to start from
     */
    ParticleIterator(Container &container, const std::size_t index)
        : container_(container), index_(index) {}

    /// @brief Handle to the current Particle
    /// @return ParticleRef for the current index
    ParticleRef<Container> operator*() const {
        return ParticleRef<Container>(container_.get(), index_);
    }

    /// @brief Increment the iterator
    /// @return this Iterator
    ParticleIterator &operator++() {
        ++index_;
        return *this;
    }

    /// @brief Compares position of the iterators, only useful for iterators over the same container
    /// @param other Iterator to compare with
    /// @return True if the indices are identical
    bool operator==(const ParticleIterator &other) const { return index_ == other.index_; }
};

/**
 * @brief Stores Particles in a Structure of Arrays (SoA)
 *
 * Every quantity is kept in its own std::vector so loops read over consecutive memory positions
 * and can therefore be vectorized.
 *
 * Because particles dont exist as objects values are read as copies (`f(i)`) and written with
 * setters (`SetF(i, ...)`).
 *
 * Iterate over with (`for (auto p : particles)` to get a handle for each particle
 */
class ParticleContainer {
    Vec3Vector x_;           ///< positions
    Vec3Vector v_;           ///< velocities
    Vec3Vector f_;           ///< forces
    Vec3Vector old_f_;       ///< old forces
    std::vector<double> m_;  ///< masses
    std::vector<int> type_;  ///< types

   public:
    /// @name Iteration
    /// @{

    /// @return Iterator from the first particle
    ParticleIterator<ParticleContainer> begin() { return {*this, 0}; }
    /// @return Iterator past the last particle
    ParticleIterator<ParticleContainer> end() { return {*this, size()}; }
    /// @return Read only iterator from the first particle
    [[nodiscard]] ParticleIterator<const ParticleContainer> begin() const { return {*this, 0}; }
    /// @return Read only Iterator past the last particle
    [[nodiscard]] ParticleIterator<const ParticleContainer> end() const { return {*this, size()}; }
    /// @}

    /**
     * @brief Appends a single particle
     * @param x Position
     * @param v Velocity
     * @param m Mass
     * @param type Type, use it for whatever you want (e.g. to separate molecules belonging to
     * different bodies, matters, and so on)
     */
    void add(const Vec3 &x, const Vec3 &v, double m, int type = 0);
    /// @return Particle count
    [[nodiscard]] std::size_t size() const { return m_.size(); }
    /**
     * @brief Reserves Memory for @p n particles to reduce amount of allocations
     *
     * @param n Expected number of particles
     */
    void reserve(std::size_t n);

    /// @name Read access by index (returns copies)
    /// @{
    [[nodiscard]] Vec3 x(const std::size_t i) const {
        return x_.get(i);
    }  ///< Position of particle i
    [[nodiscard]] Vec3 v(const std::size_t i) const {
        return v_.get(i);
    }  ///< Velocity of particle i
    [[nodiscard]] Vec3 f(const std::size_t i) const { return f_.get(i); }  ///< Force of particle i
    [[nodiscard]] Vec3 old_f(const std::size_t i) const {
        return old_f_.get(i);
    }  ///< Old Force of particle i
    [[nodiscard]] double m(const std::size_t i) const { return m_[i]; }     ///< Mass of particle i
    [[nodiscard]] int type(const std::size_t i) const { return type_[i]; }  ///< Type of particle i
    /// @}

    /// @name Write access by index
    /// @{
    void SetX(const std::size_t i, const Vec3 &x) { x_.set(i, x); }  ///< Set position of particle i
    void SetV(const std::size_t i, const Vec3 &v) { v_.set(i, v); }  ///< Set velocity of particle i
    void SetF(const std::size_t i, const Vec3 &f) { f_.set(i, f); }  ///< Set force of particle i
    void SetOldF(const std::size_t i, const Vec3 &old_f) {
        old_f_.set(i, old_f);
    }  ///< Set old force of particle i
    void SetM(const std::size_t i, const double m) { m_[i] = m; }  ///< Set mass of particle i
    void SetType(const std::size_t i, const int type) {
        type_[i] = type;
    }  ///< Set type of particle i
    /// @}

    /**
     * @brief Applies pairwise forces between all particles
     *
     * Calls the @p kernel once per unordered pair (i, j) with i < j, applying to both using
     * Newtons third law F_ij = -F_ji
     *
     * Forces are accumulated into current values of f, old f is not written. Call this after
     * resetting f to zero (potentially saving to old f)
     *
     * The inner loop is vectorized
     *
     * @warning The kernel must not have side effects and MUST NOT READ f(), that results in
     * potentially undefined behaviour because the values are written during the loop and the
     * compiler gets the independence promise
     *
     * @tparam Kernel Callable with signature
     * `Vec3(ParticleRef<const ParticleContainer> p_i, ParticleRef<const ParticleContainer> p_j)`
     *
     * @param kernel Returns the force acting on @p p_i by @p p_j
     */
    template <class Kernel>
        requires std::is_invocable_r_v<Vec3, Kernel, ParticleRef<const ParticleContainer>,
                                       ParticleRef<const ParticleContainer>>
    void ApplyPairForces(Kernel kernel) {
        const std::size_t n = size();
        for (std::size_t i = 0; i < n; ++i) {
            const ParticleRef<const ParticleContainer> p_i(*this, i);
            Vec3 f_i{};

            // Safe because every j > i is visited once per i. F_j is written so the write locations
            // dont overlap and only reads the Force it writes to in the same iteration
            INDEPENDENT_ITERATIONS
            for (std::size_t j = i + 1; j < n; j++) {
                const ParticleRef<const ParticleContainer> p_j(*this, j);
                const Vec3 f_ij = kernel(p_i, p_j);
                f_i = f_i + f_ij;
                f_.set(j, f_.get(j) - f_ij);
            }
            f_.set(i, f_.get(i) + f_i);
        }
    }

    /**
     * @brief Calculate the total energy across the system. Method used for testing.
     * Calls the @p kernel once per unordered pair (i, j) with i < j
     *
     * Kinetic energy is calculated for each particle, while Potential energy is calculated only
     * once per unordered pair
     *
     * The inner loop is vectorized
     *
     * @warning The kernel must not have side effects and MUST NOT READ f(), that results in
     * potentially undefined behaviour because the values are written during the loop and the
     * compiler gets the independence promise
     *
     * @tparam Kernel Callable with signature
     * `Vec3(ParticleRef<const ParticleContainer> p_i, ParticleRef<const ParticleContainer> p_j)`
     *
     * @param kernel Adds the potential between @p p_i and @p p_j to the accumulator @p energy
     * @param energy Reference to the accumulator for total system energy
     */
    template <class Kernel>
        requires std::is_invocable_r_v<void, Kernel, ParticleRef<const ParticleContainer>,
                                       ParticleRef<const ParticleContainer>, double &>
    void CalculateEnergy(Kernel kernel, double &energy) const {
        const std::size_t n = size();

        for (std::size_t i = 0; i < n; ++i) {
            const ParticleRef<const ParticleContainer> p_i(*this, i);
            // add the kinetic energy from this particle
            energy += 0.5 * p_i.m() * ArrayUtils::L2NormSquared(p_i.v());

            // safe as a rule since no write actions are performed except on the acculumator
            // unnecessary but included until otherwise told off
            // parallelism issues?
            INDEPENDENT_ITERATIONS
            for (std::size_t j = i + 1; j < n; ++j) {
                const ParticleRef<const ParticleContainer> p_j(*this, j);
                kernel(p_i, p_j, energy);  // add the potential energy to the accumulator energy
            }
        }
    }
};

#undef INDEPENDENT_ITERATIONS
