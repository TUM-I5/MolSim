#pragma once
#include <array>
#include <cstddef>

#include "ArrayUtils.h"
#include "ArrayVector.h"

/// Number of spatial dimensions of the simulation
constexpr std::size_t kDim = 3;

/// a single vector with kDim dimensions, arithmetic via ArrayUtils
using Vec3 = std::array<double, kDim>;
/// List of Vec3 stored as a structure of arrays, used by ParticleContainer
using Vec3Vector = ArrayVector<double, kDim>;