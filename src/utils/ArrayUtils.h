/**
 * @file ArrayMath.h
 * @author F. Gratl
 * @date 12/13/19
 */

#pragma once

#include <algorithm>
#include <array>
#include <numeric>
#include <sstream>

/**
 * Collection of utility functions and operators for iterable data containers like std::array, std::vector, etc.
 */
namespace ArrayUtils {
/**
 * Generates a string representation of a container which fulfills the Container requirement (provide cbegin and cend).
 * @tparam Container Type of Container.
 * @param container.
 * @param delimiter String that is put between items.
 * @param surround Strings to be put before and after the listing (e.g. brackets).
 * @return String representation of container.
 */
template<class Container>
[[nodiscard]] std::string to_string(const Container &container, const std::string &delimiter = ", ",
                                    const std::array<std::string, 2> &surround = {"[", "]"}) {
  auto iter = std::cbegin(container);
  const auto end = std::cend(container);
  if (iter == end) {
    return surround[0] + surround[1];
  }
  std::ostringstream strStream;
  strStream << surround[0] << *iter;
  for (++iter; iter != end; ++iter) {
    strStream << delimiter << *iter;
  }
  strStream << surround[1];
  return strStream.str();
}

/**
 * Applies an element wise binary function F to two containers.
 *
 * If the containers differ in size the F is only applied to as many elements as are in the smaller container.
 *
 * @tparam Container Type for both containers.
 * @tparam F Type of binary function.
 * @param lhs
 * @param rhs
 * @param binaryFunction
 * @return Element wise F(lhs, rhs).
 */
template<class Container, class F>
inline Container elementWisePairOp(const Container &lhs, const Container &rhs,
                                   F binaryFunction) {
  Container ret = lhs;
  auto retIter = std::begin(ret);
  auto lhsIter = std::cbegin(lhs);
  const auto lhsEnd = std::cend(lhs);
  auto rhsIter = std::cbegin(rhs);
  const auto rhsEnd = std::cend(rhs);

  for (; lhsIter != lhsEnd and rhsIter != rhsEnd;
         ++lhsIter, ++rhsIter, ++retIter) {
    *retIter = binaryFunction(*lhsIter, *rhsIter);
  }

  return ret;
}

/**
 * Applies a binary function F to with a scalar to every element in a container.
 *
 * @tparam Scalar Type of scalar value.
 * @tparam Container Type of the container.
 * @tparam F
 * @param lhs
 * @param rhs
 * @param binaryFunction
 * @return Element wise F(lhs, rhs).
 */
template<class Scalar, class Container, class F>
inline Container elementWiseScalarOp(const Scalar &lhs, const Container &rhs,
                                     F binaryFunction) {
  Container ret = rhs;
  auto retIter = std::begin(ret);
  auto rhsIter = std::cbegin(rhs);
  const auto rhsEnd = std::cend(rhs);

  for (; rhsIter != rhsEnd; ++rhsIter, ++retIter) {
    *retIter = binaryFunction(lhs, *rhsIter);
  }

  return ret;
}

/**
 * Calculates the L2 norm for a given container.
 * @tparam Container
 * @param c
 * @return sqrt(sum_i(c[i]*c[i])).
 */
template<class Container>
auto L2Norm(const Container &c) {
  return std::sqrt(std::accumulate(std::cbegin(c), std::cend(c), 0.0,
                                   [](auto a, auto b) { return a + b * b; }));
}

} // namespace ArrayUtils

/**
 * Element wise addition of two containers.
 * @tparam Container
 * @param lhs
 * @param rhs
 * @return For all i lhs[i] + rhs[i].
 */
template<class Container>
Container operator+(const Container &lhs, const Container &rhs) {
  return ArrayUtils::elementWisePairOp(lhs, rhs, std::plus<>());
}

/**
 * Element wise subtraction of two containers.
 * @tparam Container
 * @param lhs
 * @param rhs
 * @return For all i lhs[i] - rhs[i].
 */
template<class Container>
Container operator-(const Container &lhs, const Container &rhs) {
  return ArrayUtils::elementWisePairOp(lhs, rhs, std::minus<>());
}

/**
 * Element wise multiplication of two containers.
 * @tparam Container
 * @param lhs
 * @param rhs
 * @return For all i lhs[i] * rhs[i].
 */
template<class Container>
Container operator*(const Container &lhs, const Container &rhs) {
  return ArrayUtils::elementWisePairOp(lhs, rhs, std::multiplies<>());
}

/**
 * Element wise scaling of a container.
 * @tparam Container
 * @param lhs
 * @param rhs
 * @return For all i lhs * rhs[i].
 */
template<class Scalar, class Container>
Container operator*(const Scalar &lhs, const Container &rhs) {
  return ArrayUtils::elementWiseScalarOp(lhs, rhs, std::multiplies<>());
}

/**
 * Element wise comparison of two containers.
 * @tparam Container
 * @param lhs
 * @param rhs
 * @return True iff the containers are of the same size, all elements are equal, and in the same order.
 */
template<class Container>
bool operator==(const Container &lhs, const Container &rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }

  auto lhsIter = std::cbegin(lhs);
  const auto lhsEnd = std::cend(lhs);
  auto rhsIter = std::cbegin(rhs);

  for (; lhsIter != lhsEnd; ++lhsIter, ++rhsIter) {
    if (*lhsIter != *rhs) {
      return false;
    }
  }
  return true;
}
