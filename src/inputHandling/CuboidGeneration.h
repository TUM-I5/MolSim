#pragma once

#include "particleModel/ParticleContainer.h"
#include "FileReader.h"

void generateCuboid(FileReader::CuboidData& cuboid, ParticleContainer& particleContainer, size_t dim);

void addCuboids(ParticleContainer& particleContainer, std::list<FileReader::CuboidData> cuboids);