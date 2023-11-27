
#pragma once

#include "io/input/xml/parser/simulation_schema.h"
#include "particles/spawners/cuboid/CuboidSpawner.h"

class XSDTypeAdapter {
   public:
    /**
     * @brief Converts a cuboid from the XSD format to the internal format
     * @param cuboid Cuboid in the XSD format
     * @return CuboidSpawner parsed from the given cuboid in the XSD format
     */
    static CuboidSpawner convertToCuboidSpawner(const ::cuboid& cuboid);
};