//
// Created by Alp Kaan Aksu on 02.12.23.
//
#pragma once

#include <string>

enum class BoundaryBehavior {
    Periodic,
    Reflective,
    Outflow
};

std::string boundaryBehaviorToString(BoundaryBehavior boundaryBehavior);

BoundaryBehavior stringToBoundaryBehavior(const std::string &boundaryBehavior);