#pragma once

#include "particleModel/storage/SimulationContainer.h"
#include "FileReader.h"

/**
 * @brief gets global dimension of the particles and iteratively generates spheres
 *
 * Dimension is determined based on the needed z value of the domain.
 * If z_domain= 1 then it is a 2D sphere, otherwise it is 3D.
 * Capacity for the generated particles will be calculated and reserved within the particleContainer.
 * Calls generateSpheres(...) for every cuboid.
 *
 *
 * @param particleContainer reference to add particles to
 * @param spheres all the spheres data to iterate over
 * @param dim dimension of all sphere particles
 */
void addSpheres(SimulationContainer& particleContainer, std::list<FileReader::SphereData> spheres, size_t dim);



/**
 * @brief creates all particles of the Sphere and adds them into the ParticleContainer
 *
 * The given R represents the maximum number of particles on the radius.
 * Calculates the radius length by multiply R and the mesh width.
 * Creates a square/cube according to the dimension with the length 2 radius length filled with particles.
 * Then determine whether the distance of each particle to the spheres center is within the radius length
 * adding a very small number, so that the particles on the surface of the sphere are also included.
 * If so adds the particles velocity, position and mass to the particle container.
 * Maxwell boltzmann distribution is not applied here since we dont have a temperature as input.
 *
 * @param sphere data for the generation
 * @param particleContainer reference to add the spheres particles to

 */

void generateSpheresMethod2D(FileReader::SphereData& sphere, SimulationContainer& particleContainer);

void generateSpheresMethod3D(FileReader::SphereData& sphere, SimulationContainer& particleContainer);
