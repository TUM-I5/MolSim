#pragma once

#include "Eigen"

enum Shape {
    cuboid, sphere, particle, num_shapes
};
const std::array<enum Shape, num_shapes> all_shapes = {cuboid, sphere, particle};

struct Body {
    Shape shape;    /**<defines the shape of the body and therefore the interpretation of the following values*/
    Eigen::Vector3d fixpoint; /**<front bottom left point of cuboid, middle of sphere, etc*/
    Eigen::Vector3d dimensions; /**<amount of particles in each dimensional direction*/
    double distance;
    double mass;
    Eigen::Vector3d start_velocity;
};