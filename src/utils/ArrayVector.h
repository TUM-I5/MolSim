#pragma once
#include <array>
#include <cstddef>
#include <vector>

/**
 * @brief Array of Growable Vectors
 *
 * behaves like `std::vector<std::array<T,N>>` but in SoA layout so every component is contiguous.
 *
 * There is no [] because an element does not exist as one object in memory so there is no reference
 * that can be returned to. So write with set and read with get.
 *
 * @tparam T Type of a single component
 * @tparam N Number of components
 */
template <class T, std::size_t N>
class ArrayVector {
    std::array<std::vector<T>, N> values_;  ///< Storage of the values

   public:
    /**
     * @brief Appends a single value
     *
     * @param value to be appended
     */
    void push_back(const std::array<T, N> &value) {
        for (std::size_t d = 0; d < N; ++d) {
            values_[d].push_back(value[d]);
        }
    }
    /**
     * @brief Reserves Memory for @p n elements to reduce amount of allocations
     *
     * @param n Expected number of elements
     */
    void reserve(std::size_t n) {
        for (auto &component : values_) {
            component.reserve(n);
        }
    }

    /// @return number of elements
    [[nodiscard]] std::size_t size() const { return values_[0].size(); }
    /// @return element at position @p i
    /// @param i index of the value
    [[nodiscard]] std::array<T, N> get(std::size_t i) const {
        std::array<T, N> result{};
        for (std::size_t d = 0; d < N; ++d) {
            result[d] = values_[d][i];
        }
        return result;
    }
    /**
     * @brief Sets element at position i
     *
     * @param i index of the element
     * @param value new value
     */
    void set(std::size_t i, const std::array<T, N> &value) {
        for (std::size_t d = 0; d < N; ++d) {
            values_[d][i] = value[d];
        }
    }
};
