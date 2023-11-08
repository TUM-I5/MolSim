#pragma once

#include "ParticleContainer.h"
#include "FileReader.h"

namespace CuboidGenerator {
    void generateCuboid(FileReader::CuboidData& cuboidData, ParticleContainer& particleContainer);
}