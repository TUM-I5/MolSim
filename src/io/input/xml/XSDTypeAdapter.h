
#pragma once

#include "io/input/xml/simulation_schema.h"
#include "io/particle_spawners/CuboidSpawner.h"

class XSDTypeAdapter {
   public:
    static CuboidSpawner convertToCuboidSpawner(const ::cuboid& cuboid);
};