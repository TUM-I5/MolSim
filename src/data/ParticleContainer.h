#pragma once

#include "ParticleContainer.h"
#include "Particle.h"
#include "sim/physics/bounds/types.h"
#include "io/output/Logging.h"

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
    /**
     * Is in itself a 3d vector, but can provide access to a 3d sub-vector with a certain inset
     * s.t. the inner vector seems as if there is no outer vector.
     * The inset is defined by offsetXI. This offset is applied to either side individually.
     * Resulting in a total difference of 2*offset;
     * */
    class VectorCoordWrapper {
    private:
        std::vector<std::vector<unsigned long>> p_data;
        unsigned long p_dimX0;
        unsigned long p_dimX1;
        unsigned long p_dimX2;
        unsigned long p_offsetX0;
        unsigned long p_offsetX1;
        unsigned long p_offsetX2;
        unsigned long p_eDimX0;
        unsigned long p_eDimX1;
        unsigned long p_eDimX2;

    public:
        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = std::vector<unsigned long>;
            using pointer           = std::vector<unsigned long>*;
            using reference         = std::vector<unsigned long>&;

        private:
            unsigned long p_index;
            VectorCoordWrapper& p_vector;
        public:
            explicit Iterator(unsigned long i, VectorCoordWrapper& vector) : p_index(i), p_vector(vector) {}
            reference operator*() const { return p_vector.getInner(p_index); }
            pointer operator->() { return &p_vector.getInner(p_index); }

            // Prefix increment
            Iterator& operator++() { p_index++; return *this; }

            // Postfix increment
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator== (const Iterator& a, const Iterator& b) { return a.p_index == b.p_index; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.p_index != b.p_index; };
        };

        explicit VectorCoordWrapper(unsigned long dimX0 = 0, unsigned long dimX1 = 0, unsigned long dimX2 = 0,
                                    unsigned long dx0 = 1, unsigned long dx1 = 1,unsigned long dx2 = 1) :
                p_dimX0(dimX0), p_dimX1(dimX1), p_dimX2(dimX2),
                p_offsetX0(dx0), p_offsetX1(dx1), p_offsetX2(dx2),
                p_eDimX0(dimX0 - 2 * dx0), p_eDimX1(dimX1 - 2 * dx1), p_eDimX2(dimX2 - 2 * dx2) {
            p_data.resize(dimX0*dimX1*dimX2);
        }

        VectorCoordWrapper(VectorCoordWrapper& o)  noexcept : p_dimX0(o.p_dimX0), p_dimX1(o.p_dimX1), p_dimX2(o.p_eDimX2),
                p_offsetX0(o.p_offsetX0), p_offsetX1(o.p_offsetX1), p_offsetX2(o.p_offsetX2),
                p_eDimX0(o.p_eDimX0), p_eDimX1(o.p_eDimX1), p_eDimX2(o.p_eDimX2) {
            p_data.resize(o.p_data.size());
        }

        /**
         * Returns the vector at the global index.
         * */
        [[maybe_unused]] std::vector<unsigned long>& getOuter(unsigned long index) {
            return p_data[index];
        }

        /**
         * Returns the vector at the given coordinates
         * */
        [[maybe_unused]] std::vector<unsigned long>& getOuter(unsigned long x0, unsigned long x1, unsigned long x2) {
            return p_data[x0 + x1 * p_dimX0 + x2 * p_dimX0 * p_dimX1];
        }

        /**
         * Returns the vector at the given inner coordinates. \n
         * Inner to global coord conversion follows: \n
         * global: g = x0 + x1 * d0 + x2 * d0*d1 \n
         * inner : i = y0 + y1 * e0 + y2 * e0*e1 = \n
         *             (x0 + 1) + (x1 + 1) * e0 + (x2 + 1) * e0*e1 = \n
         *             (x0 + 1) + (x1 + 1) * (d0 - 2*o0) + (x2 + 1) * (d0 - 2*o0)*(d1 - 2*o1) = \n
         *             x0 + x1 * d0 + x2 * d0*d1 - 2x2d0o1 - 2x2d1o0 + 4x2o0o1 + d0d1 - 2d0o1 - 2d1o0 + 4o0o1 + 1 - x1 * 2*o0 + d0 - 2*o0 = \n
         *  ==>   g = i - (- 2x2d0o1 - 2x2d1o0 + 4x2o0o1 + d0d1 - 2d0o1 - 2d1o0 + 4o0o1 + 1 - x1 * 2*o0 + d0 - 2*o0) = \n
         *            i + 2(y2 - 1)d0o1 + 2(y2 - 1)d1o0 - 4(y2 - 1)o0o1 - d0d1 + 2d0o1 + 2d1o0 - 4o0o1 - 1 + 2(y1 - 1)o0 - d0 + 2*o0) = \n
         *            i + 2y2d0o1 - 2d0o1 + 2y2d1o0 - 2d1o0 - 4y2o0o1 + 4o0o1 - d0d1 + 2d0o1 + 2d1o0 - 4o0o1 - 1 + 2y1o0 - 2o0 - d0 + 2o0 = \n
         *            i + 2y2d0o1 + 2y2d1o0 - 4y2o0o1 - d0d1 - 1 + 2y1o0 - d0 = \n
         *  @param y0 x0 coord in inner coord system
         *  @param y1 x1 coord in inner coord system
         *  @param y2 x2 coord in inner coord system
         * */
        [[maybe_unused]] std::vector<unsigned long>& getInner(unsigned long y0, unsigned long y1, unsigned long y2) {
            unsigned long g = (y0+1)+(y1+1)*p_dimX0+(y2+1)*p_dimX0*p_dimX1;
            return p_data[g];
//            unsigned long i = y0 + y1 * p_eDimX0 + y2 * p_eDimX0 * p_eDimX1;
//            unsigned long g = i + 2 * y2 * p_dimX0 * p_offsetX1 + 2 * y2 * p_dimX1 * p_offsetX0
//                                - 4 * y2 * p_offsetX0 * p_offsetX1 - p_dimX0 * p_dimX1 - 1
//                                + 2 * y1 * p_offsetX0 - p_dimX0;
//            return p_data[g];
        }

        /**
         * Returns the vector at the given inner coordinates. \n
         * Inner to global coord conversion follows: \n
         * global: g = x0 + x1 * d0 + x2 * d0*d1 \n
         * inner : i = y0 + y1 * e0 + y2 * e0*e1 = \n
         *             (x0 + 1) + (x1 + 1) * e0 + (x2 + 1) * e0*e1 = \n
         *             (x0 + 1) + (x1 + 1) * (d0 - 2*o0) + (x2 + 1) * (d0 - 2*o0)*(d1 - 2*o1) = \n
         *             x0 + x1 * d0 + x2 * d0*d1 - 2x2d0o1 - 2x2d1o0 + 4x2o0o1 + d0d1 - 2d0o1 - 2d1o0 + 4o0o1 + 1 - x1 * 2*o0 + d0 - 2*o0 = \n
         *  ==>   g = i - (- 2x2d0o1 - 2x2d1o0 + 4x2o0o1 + d0d1 - 2d0o1 - 2d1o0 + 4o0o1 + 1 - x1 * 2*o0 + d0 - 2*o0) = \n
         *            i + 2(y2 - 1)d0o1 + 2(y2 - 1)d1o0 - 4(y2 - 1)o0o1 - d0d1 + 2d0o1 + 2d1o0 - 4o0o1 - 1 + 2(y1 - 1)o0 - d0 + 2*o0) = \n
         *            i + 2y2d0o1 - 2d0o1 + 2y2d1o0 - 2d1o0 - 4y2o0o1 + 4o0o1 - d0d1 + 2d0o1 + 2d1o0 - 4o0o1 - 1 + 2y1o0 - 2o0 - d0 + 2o0 = \n
         *            i + 2y2d0o1 + 2y2d1o0 - 4y2o0o1 - d0d1 - 1 + 2y1o0 - d0 = \n
         *  @param y0 x0 coord in inner coord system
         *  @param y1 x1 coord in inner coord system
         *  @param y2 x2 coord in inner coord system
         * */
        [[maybe_unused]] std::vector<unsigned long>& getInner(unsigned long i) {
            unsigned long y0;
            unsigned long y1;
            unsigned long y2;
            y0 = i % p_eDimX0;
            y1 = ((i - y0) % (p_eDimX0 * p_eDimX1)) / p_eDimX0;
            y2 = (i - y0 - y1 * p_eDimX0) / (p_eDimX0 * p_eDimX1);
            return getInner(y0, y1, y2);
//            y1 = (i % (p_eDimX0 * p_eDimX1)) / p_eDimX1;
//            y2 = i / (p_eDimX0 * p_eDimX1);
//            unsigned long g = i + 2 * y2 * p_dimX0 * p_offsetX1 + 2 * y2 * p_dimX1 * p_offsetX0
//                              - 4 * y2 * p_offsetX0 * p_offsetX1 - p_dimX0 * p_dimX1 - 1
//                              + 2 * y1 * p_offsetX0 - p_dimX0;
//            return p_data[g];
        }

        /**
         * delegates to getInner(i)
         * Operates on the inner 3d vector
         * */
        [[maybe_unused]] std::vector<unsigned long>& operator[](unsigned long i) {
            return getInner(i);
        }

        /**
         * Operates only on the inner 3d vector
         * */
        Iterator begin() { return Iterator(0, *this); }

        /**
         * Operates only on the inner 3d vector
         * */
        Iterator end() { return Iterator(p_eDimX0*p_eDimX1*p_eDimX2, *this); }

        /**
         * Clears all cells, also halo
         * */
        void clear() {
            p_data.clear();
        }

        /**
         * Clears all outer cells
         * */
        void clearOuter() {
            // l r
            for(unsigned long o0 = 0; o0 < p_offsetX0; o0++) {
               for(unsigned long x1 = 0; x1 < p_dimX1; x1++) {
                   for(unsigned long x2 = 0; x2 < p_dimX2; x2++) {
                       getOuter(0 + o0, x1, x2).clear();
                       getOuter(p_dimX0 - 1 - o0, x1, x2).clear();
                   }
               }
            }
            // b t
            for (unsigned long o1 = 0; o1 < p_offsetX1; o1++) {
                for(unsigned long x0 = 0; x0 < p_dimX0; x0++) {
                    for(unsigned long x2 = 0; x2 < p_dimX2; x2++) {
                        getOuter(x0, 0 + o1, x2).clear();
                        getOuter(x0, p_dimX1 - 1 - o1, x2).clear();
                    }
                }
            }
            // f r
            for (unsigned long o2 = 0; o2 < p_offsetX2; o2++){
                for(unsigned long x0 = 0; x0 < p_dimX0; x0++) {
                    for(unsigned long x1 = 0; x1 < p_dimX1; x1++) {
                        getOuter(x0, x1, 0 + o2).clear();
                        getOuter(x0, x1, p_dimX2 - 1 - o2).clear();
                    }
                }
            }
        }

        /**
         * Returns the size of the inner 3d vector
         * */
        unsigned long size() {
            return p_eDimX0*p_eDimX1*p_eDimX2;
        }

        /**
         * Stores value into cell at given coordinates.
         * Will check if it is already contained in cell.
         * Will not store a duplicate.
         * Coords are in global coord system.
         * */
         void store(unsigned long x0, unsigned long x1, unsigned long x2, unsigned long val) {
            auto& cell = getOuter(x0, x1, x2);
            if(std::find(cell.begin(), cell.end(), val) != cell.end()) return;
            cell.emplace_back(val);
         }

         /**
          * Returns an inner cell using by addressing it with the global coord system.
          * If the specified point is outside of the inner 3d vector or OOB then an empty vector is returned
          * */
          std::vector<unsigned long>& getInnerGlobal(unsigned long x0, unsigned long x1, unsigned long x2) {
              static std::vector<unsigned long> empty;
              if (x0 < p_dimX0 - p_offsetX0 - p_eDimX0 || x0 > p_offsetX0 + p_eDimX0 - 1) return empty;
              if (x1 < p_dimX1 - p_offsetX1 - p_eDimX1 || x1 > p_offsetX1 + p_eDimX1 - 1) return empty;
              if (x2 < p_dimX2 - p_offsetX2 - p_eDimX2 || x2 > p_offsetX2 + p_eDimX2 - 1) return empty;
              return getOuter(x0, x1, x2);
          }

          /**
           * Removes val from cell i
           * @param i inner dense coordinate
           * @param val item to remove
           * */
          void removeAt(unsigned long i, unsigned long val) {
              auto& cell = getInner(i);
              cell.erase(std::remove_if(cell.begin(), cell.end(), [&](const auto item) {return item == val;}), cell.end());
          }
    };

private:
    double root6_of_2;
    std::vector<double> force;
    std::vector<double> oldForce;
    std::vector<double> x;
    std::vector<double> v;
    std::vector<double> m;
    std::vector<double> eps;
    std::vector<double> sig;
    std::vector<int> type;
    unsigned long count;
    std::vector<unsigned long> activeParticles;
    VectorCoordWrapper cells;
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
    static void
    storeParticle(Particle &p, unsigned long index, std::vector<double> &force, std::vector<double> &oldForce,
                  std::vector<double> &x, std::vector<double> &v, std::vector<double> &m,
                  std::vector<int> &type, std::vector<double> &e, std::vector<double> &s);

    /**
     * Loads a particle from the internal data into @param p at @param index
     * */
    void loadParticle(Particle &p, unsigned long index);

    /**
     * Loads a particle from the provided buffer into @param p at @param index
     * */
    static void
    loadParticle(Particle &p, unsigned long index, std::vector<double> &force, std::vector<double> &oldForce,
                 std::vector<double> &x, std::vector<double> &v, std::vector<double> &m,
                 std::vector<int> &type, std::vector<double> &e, std::vector<double> &s);

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
    ParticleContainer(const std::vector<Particle> &buffer, std::array<double, 3> domainSize, double r_cutoff);

    /**
     * @brief Constructor of ParticleContainer that also initializes a seemingly two dimensional cell-structure
     *
     * @param buffer
     * @param domainSize
     * @param r_cutoff
     */
    ParticleContainer(const std::vector<Particle> &buffer, std::array<double, 2> domainSize, double r_cutoff);

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
    void
    forEachParticleHaloPairInSide(const double sigma, const std::function<void(Particle &, Particle &)> &function) {
        double maxBorderDistance = root6_of_2 * sigma / 2;
        if constexpr (S == sim::physics::bounds::side::left) {
            // left x = x_0 = min
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto &cell_indices_left = cells[cellIndexFromCellCoordinates({0, x_1, x_2})];
                    for (auto i: cell_indices_left) {
                        if (x[3 * i + 0] != 0 && x[3 * i + 0] < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            auto &pos = p_real.getX();
                            Particle p_halo = p_real;
                            p_halo.setX({(-1) * pos[0], pos[1], pos[2]});

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
                    auto &cell_indices_right = cells[cellIndexFromCellCoordinates({gridDimensions[0] - 1, x_1, x_2})];
                    for (auto i: cell_indices_right) {
                        double distance = domainSize[0] - x[3 * i + 0];
                        if (distance < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            Particle p_halo = p_real;
                            p_halo.add_to_X({distance * 2, 0, 0});

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
                    auto &cell_indices_bot = cells[cellIndexFromCellCoordinates({x_0, 0, x_2})];
                    for (auto i: cell_indices_bot) {
                        if (x[3 * i + 1] != 0 && x[3 * i + 1] < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            auto &pos = p_real.getX();
                            Particle p_halo = p_real;
                            p_halo.setX({pos[0], (-1) * pos[1], pos[2]});

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
                    auto &cell_indices_top = cells[cellIndexFromCellCoordinates({x_0, gridDimensions[1] - 1, x_2})];
                    for (auto i: cell_indices_top) {
                        double distance = domainSize[1] - x[3 * i + 1];
                        if (distance < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            Particle p_halo = p_real;
                            p_halo.add_to_X({0, distance * 2, 0});

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
                    auto &cell_indices_front = cells[cellIndexFromCellCoordinates({x_0, x_1, 0})];
                    for (auto i: cell_indices_front) {
                        if (x[3 * i + 2] != 0 && x[3 * i + 2] < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            auto &pos = p_real.getX();
                            Particle p_halo = p_real;
                            p_halo.setX({pos[0], pos[1], (-1) * pos[2]});

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
                    auto &cell_indices_back = cells[cellIndexFromCellCoordinates({x_0, x_1, gridDimensions[2] - 1})];
                    for (auto i: cell_indices_back) {
                        double distance = domainSize[2] - x[3 * i + 2];
                        if (distance < maxBorderDistance) {
                            Particle p_real;
                            loadParticle(p_real, i);
                            Particle p_halo = p_real;
                            p_halo.add_to_X({0, 0, distance * 2});

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
                    auto &cell_indices_left = cells[cellIndexFromCellCoordinates({0, x_1, x_2})];
                    for (auto i: cell_indices_left) if (x[3 * i + 0] < x_0_min) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::right) {
            // right x = x_0 = max and left x = x_0 = min
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto &cell_indices_right = cells[cellIndexFromCellCoordinates({gridDimensions[0] - 1, x_1, x_2})];
                    for (auto i: cell_indices_right) if (x[3 * i + 0] > x_0_max) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::bottom) {
            // top y = x_1 = max and bottom y = x_1 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto &cell_indices_bot = cells[cellIndexFromCellCoordinates({x_0, 0, x_2})];
                    for (auto i: cell_indices_bot) if (x[3 * i + 1] < x_1_min) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::top) {
            // top y = x_1 = max and bottom y = x_1 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto &cell_indices_top = cells[cellIndexFromCellCoordinates({x_0, gridDimensions[1] - 1, x_2})];
                    for (auto i: cell_indices_top) if (x[3 * i + 1] > x_1_max) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::front) {
            // back z = x_2 = max and front z = x_2 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                    auto &cell_indices_front = cells[cellIndexFromCellCoordinates({x_0, x_1, 0})];
                    for (auto i: cell_indices_front) if (x[3 * i + 2] < x_2_min) output.emplace(i);
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::rear) {
            // back z = x_2 = max and front z = x_2 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                    auto &cell_indices_back = cells[cellIndexFromCellCoordinates({x_0, x_1, gridDimensions[2] - 1})];
                    for (auto i: cell_indices_back) if (x[3 * i + 2] > x_2_max) output.emplace(i);
                }
            }
        }
    }

    /**
     * Removes the specified indices from the active list. For this to have effect updateCells has to be called.
     * */
    void deactivateParticles(std::unordered_set<unsigned long> &indices);

    /**
     * moves the particles at the given indices from one bound side to the opposing one.
     * will move all cell indices as well
     * */
    template<sim::physics::bounds::side S>
    void moveExternalParticles(std::unordered_set<unsigned long> &indices) {
        if constexpr (S == sim::physics::bounds::side::left) {
            // move left to right
            double delta;
            for (auto i: indices) {
                auto cellInd = cellIndexFromCellCoordinates(
                        {static_cast<unsigned int>(x[3 * i + 0]),
                         static_cast<unsigned int>(x[3 * i + 1]),
                         static_cast<unsigned int>(x[3 * i + 2])});
                cells.removeAt(cellInd, i);
                delta = x_0_min - x[3 * i + 0];
                x[3 * i + 0] = x_0_max - delta;
                cells[cellInd + gridDimensions[0] - 1].template emplace_back(i);

            }
        } else if constexpr (S == sim::physics::bounds::side::right) {
            // move right to left
            double delta;
            for (auto i: indices) {
                auto cellInd = cellIndexFromCellCoordinates(
                        {static_cast<unsigned int>(x[3 * i + 0]),
                         static_cast<unsigned int>(x[3 * i + 1]),
                         static_cast<unsigned int>(x[3 * i + 2])});
                cells.removeAt(cellInd, i);
                delta = x[3 * i + 0] - x_0_max;
                x[3 * i + 0] = x_0_min + delta;
                cells[cellInd - gridDimensions[0] + 1].template emplace_back(i);
            }
        } else if constexpr (S == sim::physics::bounds::side::bottom) {
            // move bot to top
            double delta;
            for (auto i: indices) {
                auto cellInd = cellIndexFromCellCoordinates(
                        {static_cast<unsigned int>(x[3 * i + 0]),
                         static_cast<unsigned int>(x[3 * i + 1]),
                         static_cast<unsigned int>(x[3 * i + 2])});
                cells.removeAt(cellInd, i);
                delta = x_1_min - x[3 * i + 1];
                x[3 * i + 1] = x_1_max - delta;
                cells[cellInd + gridDimensions[0] * (gridDimensions[1] - 1)].template emplace_back(i);
            }
        } else if constexpr (S == sim::physics::bounds::side::top) {
            // move top to bot
            double delta;
            for (auto i: indices) {
                auto cellInd = cellIndexFromCellCoordinates(
                        {static_cast<unsigned int>(x[3 * i + 0]),
                         static_cast<unsigned int>(x[3 * i + 1]),
                         static_cast<unsigned int>(x[3 * i + 2])});
                cells.removeAt(cellInd, i);
                delta = x[3 * i + 1] - x_1_max;
                x[3 * i + 1] = x_1_min + delta;
                cells[cellInd - gridDimensions[0] * (gridDimensions[1] - 1)].template emplace_back(i);
            }
        } else if constexpr (S == sim::physics::bounds::side::front) {
            // move front to rear
            double delta;
            for (auto i: indices) {
                auto cellInd = cellIndexFromCellCoordinates(
                        {static_cast<unsigned int>(x[3 * i + 0]),
                         static_cast<unsigned int>(x[3 * i + 1]),
                         static_cast<unsigned int>(x[3 * i + 2])});
                cells.removeAt(cellInd, i);
                delta = x_2_min - x[3 * i + 2];
                x[3 * i + 2] = x_2_max - delta;
                cells[cellInd + gridDimensions[0] * gridDimensions[1] * (gridDimensions[2] - 1)].template emplace_back(i);
            }
        } else if constexpr (S == sim::physics::bounds::side::rear) {
            // move rear to front
            double delta;
            for (auto i: indices) {
                auto cellInd = cellIndexFromCellCoordinates(
                        {static_cast<unsigned int>(x[3 * i + 0]),
                         static_cast<unsigned int>(x[3 * i + 1]),
                         static_cast<unsigned int>(x[3 * i + 2])});
                cells.removeAt(cellInd, i);
                delta = x[3 * i + 2] - x_2_max;
                x[3 * i + 2] = x_2_min + delta;
                cells[cellInd - gridDimensions[0] * gridDimensions[1] * (gridDimensions[2] - 1)].template emplace_back(i);
            }
        }
    }

    /**
     * Stores all particles, that are within the threshold of sixth root of 2 times sigma, into the halo at the opposing side.
     * */
    template<sim::physics::bounds::side S>
    void storeBorderParticlesToHalo(double sigma, bool mMinor, bool mMajor) {
        double maxBorderDistance = root6_of_2 * sigma / 2;
        double x0, x1, x2;

        if constexpr (S == sim::physics::bounds::side::left) {
            // left x = x_0 = min
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto &cell_indices_left = cells[cellIndexFromCellCoordinates({0, x_1, x_2})];
                    for (auto i: cell_indices_left) {
                        x0 = x[3 * i + 0];
                        x1 = x[3 * i + 1];
                        x2 = x[3 * i + 2];
                        if (x0 != 0 && x0 < maxBorderDistance) {
                            double dTop, dBot, dFront, dRear;
                            dTop = x_1_max - x1;
                            dBot = x1 - x_1_min;
                            dRear = x_2_max - x2;
                            dFront = x2 - x_2_min;
                            unsigned long g0 = gridDimensions[0] + 1;
                            unsigned long g1 = x_1 + 1
                                    - mMinor * (1 - static_cast<int>(dTop/maxBorderDistance)) * gridDimensions[1]
                                    + mMinor * (1 - static_cast<int>(dBot/maxBorderDistance)) * gridDimensions[1];
                            unsigned long g2 = x_2 + 1
                                    - mMajor * (1 - static_cast<int>(dRear/maxBorderDistance)) * gridDimensions[2]
                                    + mMajor * (1 - static_cast<int>(dFront/maxBorderDistance)) * gridDimensions[2];
                            cells.store(g0, g1, g2, i);
                        } // x0 check
                    }
                }
            }
        }
        else if constexpr (S == sim::physics::bounds::side::right) {
            // right x = x_0 = max and left x = x_0 = min
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto &cell_indices_right = cells[cellIndexFromCellCoordinates({gridDimensions[0] - 1, x_1, x_2})];
                    for (auto i: cell_indices_right) {
                        x0 = x[3 * i + 0];
                        x1 = x[3 * i + 1];
                        x2 = x[3 * i + 2];
                        if (domainSize[0] - x0 < maxBorderDistance) {
                            double dTop, dBot, dFront, dRear;
                            dTop = x_1_max - x1;
                            dBot = x1 - x_1_min;
                            dRear = x_2_max - x2;
                            dFront = x2 - x_2_min;
                            unsigned long g0 = 0;
                            unsigned long g1 = x_1 + 1
                                               - mMinor * (1 - static_cast<int>(dTop/maxBorderDistance)) * gridDimensions[1]
                                               + mMinor * (1 - static_cast<int>(dBot/maxBorderDistance)) * gridDimensions[1];
                            unsigned long g2 = x_2 + 1
                                               - mMajor * (1 - static_cast<int>(dRear/maxBorderDistance)) * gridDimensions[2]
                                               + mMajor * (1 - static_cast<int>(dFront/maxBorderDistance)) * gridDimensions[2];
                            cells.store(g0, g1, g2, i);
                        } // x0 check
                    }
                }
            }
        }
        else if constexpr (S == sim::physics::bounds::side::bottom) {
            // top y = x_1 = max and bottom y = x_1 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto &cell_indices_bot = cells[cellIndexFromCellCoordinates({x_0, 0, x_2})];
                    for (auto i: cell_indices_bot) {
                        x0 = x[3 * i + 0];
                        x1 = x[3 * i + 1];
                        x2 = x[3 * i + 2];
                        if (x1 != 0 && x1 < maxBorderDistance) {
                            double dLeft, dRight, dFront, dRear;
                            dRight = x_0_max - x0;
                            dLeft = x0 - x_0_min;
                            dRear = x_2_max - x2;
                            dFront = x_2_min - x2;
                            unsigned long g0 = x_0 + 1
                                               - mMinor * (1 - static_cast<int>(dRight/maxBorderDistance)) * gridDimensions[0]
                                               + mMinor * (1 - static_cast<int>(dLeft/maxBorderDistance)) * gridDimensions[0];
                            unsigned long g1 = gridDimensions[1] + 1;
                            unsigned long g2 = x_2 + 1
                                               - mMajor * (1 - static_cast<int>(dRear/maxBorderDistance)) * gridDimensions[2]
                                               + mMajor * (1 - static_cast<int>(dFront/maxBorderDistance)) * gridDimensions[2];
                            cells.store(g0, g1, g2, i);
                        } // x0 check
                    }
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::top) {
            // top y = x_1 = max and bottom y = x_1 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                    auto &cell_indices_top = cells[cellIndexFromCellCoordinates({x_0, gridDimensions[1] - 1, x_2})];
                    for (auto i: cell_indices_top) {
                        x0 = x[3 * i + 0];
                        x1 = x[3 * i + 1];
                        x2 = x[3 * i + 2];
                        if (domainSize[1] - x1 < maxBorderDistance) {
                            double dLeft, dRight, dFront, dRear;
                            dRight = x_0_max - x0;
                            dLeft = x0 - x_0_min;
                            dRear = x_2_max - x2;
                            dFront = x_2_min - x2;
                            unsigned long g0 = x_0 + 1
                                               - mMinor * (1 - static_cast<int>(dRight/maxBorderDistance)) * gridDimensions[0]
                                               + mMinor * (1 - static_cast<int>(dLeft/maxBorderDistance)) * gridDimensions[0];
                            unsigned long g1 = 0;
                            unsigned long g2 = x_2 + 1
                                               - mMajor * (1 - static_cast<int>(dRear/maxBorderDistance)) * gridDimensions[2]
                                               + mMajor * (1 - static_cast<int>(dFront/maxBorderDistance)) * gridDimensions[2];
                            cells.store(g0, g1, g2, i);
                        } // x0 check
                    }
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::front) {
            // back z = x_2 = max and front z = x_2 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                    auto &cell_indices_front = cells[cellIndexFromCellCoordinates({x_0, x_1, 0})];
                    for (auto i: cell_indices_front) {
                        x0 = x[3 * i + 0];
                        x1 = x[3 * i + 1];
                        x2 = x[3 * i + 2];
                        if (x2 != 0 && x2 < maxBorderDistance) {
                            double dTop, dBot, dLeft, dRight;
                            dTop = x_1_max - x1;
                            dBot = x_1_min - x1;
                            dRight = x_0_max - x0;
                            dLeft = x0 - x_0_min;
                            unsigned long g0 = x_0 + 1
                                               - mMinor * (1 - static_cast<int>(dRight/maxBorderDistance)) * gridDimensions[0]
                                               + mMinor * (1 - static_cast<int>(dLeft/maxBorderDistance)) * gridDimensions[0];
                            unsigned long g1 = x_1 + 1
                                               - mMajor * (1 - static_cast<int>(dTop/maxBorderDistance)) * gridDimensions[1]
                                               + mMajor * (1 - static_cast<int>(dBot/maxBorderDistance)) * gridDimensions[1];
                            unsigned long g2 = gridDimensions[2] + 1;
                            cells.store(g0, g1, g2, i);
                        } // x0 check
                    }
                }
            }
        } else if constexpr (S == sim::physics::bounds::side::rear) {
            // back z = x_2 = max and front z = x_2 = min
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                    auto &cell_indices_back = cells[cellIndexFromCellCoordinates({x_0, x_1, gridDimensions[2] - 1})];
                    for (auto i: cell_indices_back) {
                        x0 = x[3 * i + 0];
                        x1 = x[3 * i + 1];
                        x2 = x[3 * i + 2];
                        if (domainSize[2] - x2 < maxBorderDistance) {
                            double dTop, dBot, dLeft, dRight;
                            dTop = x_1_max - x1;
                            dBot = x_1_min - x1;
                            dRight = x_0_max - x0;
                            dLeft = x0 - x_0_min;
                            unsigned long g0 = x_0 + 1
                                               - mMinor * (1 - static_cast<int>(dRight/maxBorderDistance)) * gridDimensions[0]
                                               + mMinor * (1 - static_cast<int>(dLeft/maxBorderDistance)) * gridDimensions[0];
                            unsigned long g1 = x_1 + 1
                                               - mMajor * (1 - static_cast<int>(dTop/maxBorderDistance)) * gridDimensions[1]
                                               + mMajor * (1 - static_cast<int>(dBot/maxBorderDistance)) * gridDimensions[1];
                            unsigned long g2 = 0;
                            cells.store(g0, g1, g2, i);
                        } // x0 check
                    }
                }
            }
        }
    }

    /**
     * Clears all halo entries
     * */
     void clearHalo() {
         cells.clearOuter();
     }

    /**
     * @brief getter for gridDimensions.
     * There are gridDimensions[0]*gridDimensions[1]*gridDimensions[2] cells used
     *
     */
    std::array<unsigned int, 3> getGridDimensions();

    /**
     * Performs fun once. Provides all internal data to the lambda.
     * */
    template<typename F>
    void runOnDataCell(F fun) {
        fun(force, oldForce, x, v, m, type, count, cells, eps, sig);
    }

    /**
     * Runs the function on the internal data
     * */
    template<typename F>
    void runOnData(F fun) {
        fun(force, oldForce, x, v, m, type, count, eps, sig);
    }

    /**
     * @brief Applies the given function to all Particles
     *
     * @param function
     */
    void forAllParticles(const std::function<void(Particle &)> &function);

    /**
     * @brief Applies the given function to all Particles
     *
     * @param function
     */
    void forAllParticles(void(*function)(Particle &));

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
    void forAllPairs(const std::function<void(Particle &p1, Particle &p2)> &function);

    /**
     * Handles interactions between halo and border cells.
     * Runs fun on ever particle pair with one particle being in the halo and the other in the border.
     * */
     template<typename F>
     void forAllPairsHalo(F fun) {
         //handle corners
        {
            //pair<halo_coords,border_coords>
            using p = std::pair<std::array<unsigned long, 3>, std::array<unsigned long, 3>>;
            std::array<p, 8> corners {
                //front
                    p{{0L, 0L, 0L}, {1L, 1L, 1L}},
                    p{{gridDimensions[0] + 1, 0, 0}, {gridDimensions[0], 1, 1}},
                    p{{0, gridDimensions[1] + 1, 0}, {1,gridDimensions[1], 1}},
                    p{{gridDimensions[0] + 1, gridDimensions[1] + 1, 0}, {gridDimensions[0], gridDimensions[1], 1}},
                //rear
                    p{{0, 0, gridDimensions[2] + 1}, {1, 1, gridDimensions[2]}},
                    p{{gridDimensions[0] + 1, 0, gridDimensions[2] + 1}, {gridDimensions[0], 1, gridDimensions[2]}},
                    p{{0, gridDimensions[1] + 1, gridDimensions[2] + 1}, {1, gridDimensions[1], gridDimensions[2]}},
                    p{{gridDimensions[0] + 1, gridDimensions[1] + 1, gridDimensions[2] + 1}, {gridDimensions[0], gridDimensions[1], gridDimensions[2]}}
            };

            for(auto& [haloCoords, borderCoords] : corners) {
                auto& hCell = cells.getOuter(haloCoords[0], haloCoords[1], haloCoords[2]);
                auto& bCell = cells.getOuter(borderCoords[0], borderCoords[1], borderCoords[2]);
                for(unsigned long h : hCell) {
                    Particle hP;
                    loadParticle(hP, h);
                    hP.add_to_X({((haloCoords[0])?domainSize[0]:-domainSize[0]),
                                 ((haloCoords[1])?domainSize[1]:-domainSize[1]),
                                 ((haloCoords[2])?domainSize[2]:-domainSize[2])});
                    for(unsigned long b : bCell) {
                        Particle bP;
                        loadParticle(bP, b);
                        fun(bP,hP);
                        storeParticle(bP, b);
                    }
                }
            }
        } // handle corners
        //handle edges
        {
            // tuple<starting_point, dimension, check_direction>
            using t = const std::tuple<std::array<unsigned long, 3>, std::array<unsigned long, 3>, std::array<int, 3>>;
            const std::array<t, 12> edges {
                //left to right
                t{{1,0,0},{gridDimensions[0],1,1},{0,1,1}},
                t{{1,gridDimensions[1]+1,0},{gridDimensions[0],1,1},{0,-1,1}},
                t{{1,0,gridDimensions[2]+1},{gridDimensions[0],1,1},{0,1,-1}},
                t{{1,gridDimensions[1]+1,gridDimensions[2]+1},{gridDimensions[0],1,1},{0,-1,-1}},
                //bottom to top
                t{{0,1,0},{1,gridDimensions[1],1},{1,0,1}},
                t{{gridDimensions[0]+1,1,0},{1,gridDimensions[1],1},{-1,0,1}},
                t{{0,1,gridDimensions[2]+1},{1,gridDimensions[1],1},{1,0,-1}},
                t{{gridDimensions[0]+1,1,gridDimensions[2]+1},{1,gridDimensions[1],1},{-1,0,-1}},
                //front to rear
                t{{0,0,1},{1,1,gridDimensions[2]},{1,1,0}},
                t{{gridDimensions[0]+1,0,1},{1,1,gridDimensions[2]},{-1,1,0}},
                t{{0,gridDimensions[1]+1,1},{1,1,gridDimensions[2]},{1,-1,0}},
                t{{gridDimensions[0]+1,gridDimensions[1]+1,+1},{1,1,gridDimensions[2]},{-1,-1,0}}
            };

            for(auto& [start, dim, dir] : edges) {
                for(unsigned long x0 {start[0]}; x0 < start[0] + dim[0]; x0++) {
                    for(unsigned long x1 {start[1]}; x1 < start[1] + dim[1]; x1++){
                        for(unsigned long x2 {start[2]}; x2 < start[2] + dim[2]; x2++){
                            auto& hCell = cells.getOuter(x0, x1, x2);
                            //go in check direction
                            auto& bCell = cells.getOuter(x0 + dir[0], x1 + dir[1], x2 + dir[2]);
                            //handle diagonals
                            static constexpr std::array<int, 3> ones {1,1,1}; // only one is 0, rest -1 or 1
                            const std::array<int, 3> posDir {ones[0] & dir[0], ones[1] & dir[1], ones[2] & dir[2]}; // only one is 0, rest 1
                            const std::array<int, 3> walkDir {1 - posDir[0], 1 - posDir[1], 1 - posDir[2]}; // only one is 1 rest 0
                            std::vector<unsigned long>* bCellNext = nullptr;
                            std::vector<unsigned long>* bCellPrev = nullptr;
                            //go in diag to walk direction + check direction
                            if ((walkDir[0]&&(x0<start[0]+dim[0]-1)) ||
                                (walkDir[1]&&(x1<start[1]+dim[1]-1)) ||
                                (walkDir[2]&&(x2<start[2]+dim[2]-1))) {
                                bCellNext = &cells.getOuter(x0 + dir[0] + walkDir[0], x1 + dir[1] + walkDir[1], x2 + dir[2] + walkDir[2]);
                            }
                            //go in reverse diag to walk direction + check direction
                            if ((walkDir[0]&&(x0>start[0])) ||
                                (walkDir[1]&&(x1>start[1])) ||
                                (walkDir[2]&&(x2>start[2]))) {
                                bCellPrev = &cells.getOuter(x0 + dir[0] - walkDir[0], x1 + dir[1] - walkDir[1], x2 + dir[2] - walkDir[2]);
                            }

                            //perform fun
                            for(unsigned long h : hCell) {
                                Particle hP;
                                loadParticle(hP, h);
                                hP.add_to_X({(-domainSize[0] * dir[0]),
                                             (-domainSize[1] * dir[1]),
                                             (-domainSize[2] * dir[2])});
                                for(unsigned long b : bCell) {
                                    Particle bP;
                                    loadParticle(bP, b);
                                    fun(bP, hP);
                                    storeParticle(bP, b);
                                }
                                if(bCellNext) {
                                    for(unsigned long b : *bCellNext) {
                                        Particle bP;
                                        loadParticle(bP, b);
                                        fun(bP, hP);
                                        storeParticle(bP, b);
                                    }
                                }
                                if(bCellPrev) {
                                    for(unsigned long b : *bCellPrev) {
                                        Particle bP;
                                        loadParticle(bP, b);
                                        fun(bP, hP);
                                        storeParticle(bP, b);
                                    }
                                }
                            }


                        }
                    }
                }
            }
        }//handle edges
        //handle planes
        {
            // tuple<starting_point, dimension, check_direction>
            using a = std::array<int, 3>;
            using m = std::array<a, 9>;
            using t = const std::tuple<std::array<unsigned long, 3>, std::array<unsigned long, 3>, m>;
            const std::array<t, 6> planes {
                    //left
                    t{{0,1,1},{1,gridDimensions[1],gridDimensions[2]},m{a{1,1,1}, {1,1,0}, {1,1,-1},
                                                                        a{1,0,1}, {1,0,0}, {1,0,-1},
                                                                        a{1,-1,1},{1,-1,0},{1,-1,-1}}},
                    //right
                    t{{gridDimensions[0]+1,1,1},{1,gridDimensions[1],gridDimensions[2]},m{a{-1,1,1}, {-1,1,0}, {-1,1,-1},
                                                                                          a{-1,0,1}, {-1,0,0}, {-1,0,-1},
                                                                                          a{-1,-1,1},{-1,-1,0},{-1,-1,-1}}},
                    //bottom
                    t{{1,0,1},{gridDimensions[0],1,gridDimensions[2]},m{a{-1,1,1}, {0,1,1}, {1,1,1},
                                                                        a{-1,1,0}, {0,1,0}, {1,1,0},
                                                                        a{-1,1,-1},{0,1,-1},{1,1,-1}}},
                    //top
                    t{{1,gridDimensions[1]+1,1},{gridDimensions[0],1,gridDimensions[2]},m{a{-1,-1,1}, {0,-1,1}, {1,-1,1},
                                                                                          a{-1,-1,0}, {0,-1,0}, {1,-1,0},
                                                                                          a{-1,-1,-1},{0,-1,-1},{1,-1,-1}}},
                    //front
                    t{{1,1,0},{gridDimensions[0],gridDimensions[1],1},m{a{-1,1,1}, {0,1,1}, {1,1,1},
                                                                        a{-1,0,1}, {0,0,1}, {1,0,1},
                                                                        a{-1,-1,1},{0,-1,1},{1,-1,1}}},
                    //rear
                    t{{1,1,gridDimensions[2]+1},{gridDimensions[0],gridDimensions[1],1},m{a{-1,1,-1}, {0,1,-1}, {1,1,-1},
                                                                                          a{-1,0,-1}, {0,0,-1}, {1,0,-1},
                                                                                          a{-1,-1,-1},{0,-1,-1},{1,-1,-1}}}
            };

            for(auto& [start, dim, dirs] : planes) {
                for(unsigned long x0 {start[0]}; x0 < start[0] + dim[0]; x0++) {
                    for(unsigned long x1 {start[1]}; x1 < start[1] + dim[1]; x1++){
                        for(unsigned long x2 {start[2]}; x2 < start[2] + dim[2]; x2++){
                            auto& hCell = cells.getOuter(x0, x1, x2);
                            for(const auto& dir : dirs) {
                                auto& mirror = dirs[4];
                                //go in check direction
                                auto& bCell = cells.getInnerGlobal(x0 + dir[0], x1 + dir[1], x2 + dir[2]);
                                //perform fun
                                for(unsigned long h : hCell) {
                                    Particle hP;
                                    loadParticle(hP, h);
                                    hP.add_to_X({(-domainSize[0] * mirror[0]),
                                                 (-domainSize[1] * mirror[1]),
                                                 (-domainSize[2] * mirror[2])});
                                    for(unsigned long b : bCell) {
                                        Particle bP;
                                        loadParticle(bP, b);
                                        fun(bP, hP);
                                        storeParticle(bP, b);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }//handle planes
     }

    /**
     * Performs fun on provided data. All lambda args particle container internal data.
     * Will be applied on every cell.
     * */
    template<typename F>
    void forAllCells(F fun) {
        for (auto &cellItems: cells) {
            fun(force, oldForce, x, v, m, type, count, cellItems, eps, sig);
        }
    }

    /**
     * @brief Performs function on all Pairs that are in the same cell
     * 
     * @param function 
     */
    void forAllPairsInSameCell(const std::function<void(Particle &p1, Particle &p2)> &function);

    /**
     * @brief Performs function on all Pairs that are in neighbouring cells. Does not perform fun if 2 particles are in the same cell
     * 
     * @param function 
     */
    void forAllPairsInNeighbouringCell(const std::function<void(Particle &p1, Particle &p2)> &function);

    /**
     * Performs fun on provided data. All lambda args particle container internal data.
     * Will be applied on every distinct cell pair. (Set-Wise) I.e. {a,b} = {b,a}.
     * */
    [[maybe_unused]] [[deprecated]] void forAllDistinctCellPairs(void (*fun)(std::vector<double> &force,
                                                                             std::vector<double> &oldForce,
                                                                             std::vector<double> &x,
                                                                             std::vector<double> &v,
                                                                             std::vector<double> &m,
                                                                             std::vector<int> &type,
                                                                             unsigned long count,
                                                                             std::vector<unsigned long> &cell0Items,
                                                                             std::vector<unsigned long> &cell1Items));


    /**
     * Performs fun on provided data. All lambda args particle container internal data.
     * Will be applied on every distinct cell neighbours. (Set-Wise) I.e. {a,b} = {b,a}.
     * Arguments:
     *
     * std::vector<double> &force,
     * std::vector<double> &oldForce,
     * std::vector<double> &x,
     * std::vector<double> &v,
     * std::vector<double> &m,
     * std::vector<int> &type,
     * unsigned long count,
     * std::vector<unsigned long> &cell0Items
     * std::vector<unsigned long> &cell1Items
     * std::vector<double> &eps,
     * std::vector<double> &sig
     * */
    template<typename F>
    void forAllDistinctCellNeighbours(F fun) {
        //Implementation2:
        //basically every code snippet occurs three times right here because every dimension needs to bee the "free variable" for every case once
        //but actually this seems more robust than making some fancy "iterate over all possible variable distribution"-thingies

        //Straight lines ----------------------------------------
        //all pairs in x_0 direction:
        for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
            for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0 + 1, x_1, x_2)], eps, sig);
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2, x_0 + 1, x_1, x_2);
                }
            }
        }
        //all pairs in x_1 direction:
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
            for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1 + 1, x_2)], eps, sig);
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2, x_0,
                                 x_1 + 1, x_2);
                }
            }
        }
        //all pairs in x_2 direction:
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2 + 1)], eps, sig);
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2, x_0,
                                 x_1, x_2 + 1);
                }
            }
        }
        //End of straight lines ---------------------------------------------------

        //"2d-diagonals"------------------------------------------------
        //diagonals lying in the x_0-x_1 plane
        for (unsigned int x_2 = 0; x_2 < gridDimensions[2]; x_2++) {
            //diagonals from bottom left to top right
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
                for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0 + 1, x_1 + 1,
                                                               x_2)], eps,
                        sig); //check with the neighbour that is one to the right and one above you
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                 x_0 + 1, x_1 + 1, x_2);
                }
            }
            //diagonals from top left to bottom right
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
                for (unsigned int x_1 = 1; x_1 < gridDimensions[1]; x_1++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0 + 1, x_1 - 1,
                                                               x_2)], eps,
                        sig); //(check with the neighbour that is one to the right and one below you)
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                 x_0 + 1, x_1 - 1, x_2);
                }
            }
        }
        //diagonals lying in the x_0-x_2 plane
        for (unsigned int x_1 = 0; x_1 < gridDimensions[1]; x_1++) {
            //diagonals from bottom left to top right
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0 + 1, x_1, x_2 +
                                                                             1)], eps,
                        sig); //check with the neighbour that is one to the right and one above you
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                 x_0 + 1, x_1, x_2 + 1);
                }
            }
            //diagonals from top left to bottom right
            for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
                for (unsigned int x_2 = 1; x_2 < gridDimensions[2]; x_2++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0 + 1, x_1, x_2 -
                                                                             1)], eps,
                        sig); //(check with the neighbour that is one to the right and one below you)
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                 x_0 + 1, x_1, x_2 - 1);
                }
            }
        }
        //diagonals lying in the x_1-x_2 plane
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0]; x_0++) {
            //diagonals from bottom left to top right
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1 + 1, x_2 +
                                                                             1)], eps,
                        sig); //check with the neighbour that is one to the right and one above you
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2, x_0,
                                 x_1 + 1, x_2 + 1);
                }
            }
            //diagonals from top left to bottom right
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
                for (unsigned int x_2 = 1; x_2 < gridDimensions[2]; x_2++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1 + 1, x_2 -
                                                                             1)], eps,
                        sig); //(check with the neighbour that is one to the right and one below you)
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2, x_0,
                                 x_1 + 1, x_2 - 1);
                }
            }
        }
        //End of "2d diagonals"-----------------------------------------------

        //Start of "3d diagonals"----------------
        //from bottom front left top back right
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0 + 1, x_1 + 1, x_2 + 1)], eps, sig);
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                 x_0 + 1, x_1 + 1, x_2 + 1);
                    //std::cout<<"(" << x_0 << ", " << x_1 << ", " << x_2 << ") interacted with (" << x_0+1 << ", " << x_1+1 << ", " << x_2+1 << ")\n";
                }
            }
        }
        //from top front left to bottom back right
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
            for (unsigned int x_1 = 1; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 0; x_2 < gridDimensions[2] - 1; x_2++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0 + 1, x_1 - 1, x_2 + 1)], eps, sig);
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                 x_0 + 1, x_1 - 1, x_2 + 1);
                }
            }
        }
        //from bottom back left to top front right
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
            for (unsigned int x_1 = 0; x_1 < gridDimensions[1] - 1; x_1++) {
                for (unsigned int x_2 = 1; x_2 < gridDimensions[2]; x_2++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0 + 1, x_1 + 1, x_2 - 1)], eps, sig);
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                 x_0 + 1, x_1 + 1, x_2 - 1);
                }
            }
        }
        //from top back left to bottom front right
        for (unsigned int x_0 = 0; x_0 < gridDimensions[0] - 1; x_0++) {
            for (unsigned int x_1 = 1; x_1 < gridDimensions[1]; x_1++) {
                for (unsigned int x_2 = 1; x_2 < gridDimensions[2]; x_2++) {
                    fun(force, oldForce, x, v, m, type, count,
                        cells[cellIndexFromCellCoordinatesFast(x_0, x_1, x_2)],
                        cells[cellIndexFromCellCoordinatesFast(x_0 + 1, x_1 - 1, x_2 - 1)], eps, sig);
                    SPDLOG_TRACE("Cell ({} {} {}) interacted with ({} {} {})", x_0, x_1, x_2,
                                 x_0 + 1, x_1 - 1, x_2 - 1);
                }
            }
        }
        //End of "3d diagonals" -----------------
    }

};