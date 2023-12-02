//
// Created by Alp Kaan Aksu on 02.12.23.
//

#include "BoundaryBehavior.h"

std::string boundaryBehaviorToString(BoundaryBehavior boundaryBehavior) {
    switch (boundaryBehavior) {
        case BoundaryBehavior::Periodic:
            return "Periodic";
        case BoundaryBehavior::Reflective:
            return "Reflective";
        case BoundaryBehavior::Outflow:
            return "Outflow";
    }
}

BoundaryBehavior stringToBoundaryBehavior(const std::string &boundaryBehavior) {
    if (boundaryBehavior == "periodic") {
        return BoundaryBehavior::Periodic;
    } else if (boundaryBehavior == "reflective") {
        return BoundaryBehavior::Reflective;
    } else if (boundaryBehavior == "outflow") {
        return BoundaryBehavior::Outflow;
    } else {
        throw std::runtime_error("Invalid boundary behavior: " + boundaryBehavior);
    }
}

