#pragma once

#include <variant>

#include "io/input/xml/parser/simulation_schema.h"
#include "particles/containers/linkedcells/LinkedCellsContainer.h"
#include "particles/spawners/cuboid/CuboidSpawner.h"
#include "particles/spawners/sphere/SphereSpawner.h"
#include "simulation/SimulationParams.h"

/**
 * @brief Class to convert XSD types to internal types
 */
class XSDTypeAdapter {
   public:
    /**
     * @brief Converts a cuboid from the XSD format to the internal format
     *
     * @param cuboid Cuboid in the XSD format
     * @return CuboidSpawner parsed from the given cuboid in the XSD format
     */
    static CuboidSpawner convertToCuboidSpawner(const particles::cuboid_spawner_type& cuboid);

    /**
     * @brief Converts a sphere from the XSD format to the internal format
     *
     * @param sphere Sphere in the XSD format
     * @return SphereSpawner parsed from the given sphere in the XSD format
     */
    static SphereSpawner convertToSphereSpawner(const particles::sphere_spawner_type& sphere);

    /**
     * @brief Converts a particle from the XSD format to the internal format
     *
     * @param particle Particle in the XSD format
     * @return Particle parsed from the given particle in the XSD format
     */
    static Particle convertToParticle(const particles::single_particle_type& particle);

    /**
     * @brief Converts a container type from the XSD format to the internal format
     *
     * @param container_type Container type in the XSD format
     * @return ContainerType parsed from the given container type in the XSD format
     */
    static std::variant<SimulationParams::DirectSumType, SimulationParams::LinkedCellsType> convertToParticleContainer(
        const settings::particle_container_type& container_type);

    /**
     * @brief Converts a boundary conditions type from the XSD format to the internal format
     *
     * @param boundary Boundary conditions type in the XSD format
     * @return BoundaryConditionsArray parsed from the given boundary conditions type in the XSD format
     */
    static std::array<LinkedCellsContainer::BoundaryCondition, 6> convertToBoundaryConditionsArray(const BoundaryConditionsType& boundary);

    /**
     * @brief Converts a boundary type from the XSD format to the internal format
     *
     * @param boundary Boundary condition in the XSD format
     * @return BoundaryCondition parsed from the given boundary type in the XSD format
     */
    static LinkedCellsContainer::BoundaryCondition convertToBoundaryCondition(const BoundaryType& boundary);

    /**
     * @brief Converts a double vector from the XSD format to the internal format
     *
     * @param vector double Vector in the XSD format
     * @return Vector parsed from the given vector in the XSD format
     */
    static std::array<double, 3> convertToVector(const DoubleVec3Type& vector);

    /**
     * @brief Converts a int vector from the XSD format to the internal format
     *
     * @param vector int Vector in the XSD format
     * @return Vector parsed from the given vector in the XSD format
     */
    static std::array<int, 3> convertToVector(const IntVec3Type& vector);
};