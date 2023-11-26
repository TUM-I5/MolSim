#pragma once

#include "particleModel/storage/ParticleContainer.h"
#include "FileReader.h"


/**
 * @brief creates all particles of the cuboid and adds them into the ParticleContainer
 *
 * iterates over each dimension N of the cuboid and creates particles with given
 * dimension and the cuboids initial velocity, mass and coordinates. The coordinates
 * offset is based on the particles location within the cuboid and a distance h
 * to the particles around it. Maxwell boltzmann distribution is applied on the
 * initial velocity with the cuboids motion average and the particles dimension.
 *
 * @param cuboid data for the generation
 * @param particleContainer reference to add the cuboids particles to
 * @param dim dimension of all cuboids particles
 */
void generateCuboid(FileReader::CuboidData& cuboid, ParticleContainer& particleContainer, size_t dim);


/**
 * @brief determines global dimension of the cuboids particles and iteratively generates cuboids
 *
 * dimension is determined based on differences among the z axis or a velocity
 * on the z axis != 0. Capacity for the generated particles will be calculated
 * and reserved within the particleContainer. Calls generateCuboid(...) for every
 * cuboid.
 *
 * @param particleContainer reference to add particles to
 * @param cuboids all the cuboids data to iterate over
 */
void addCuboids(ParticleContainer& particleContainer, std::list<FileReader::CuboidData> cuboids);