//
// Created by Alp Kaan Aksu on 02.12.23.
//
#pragma once

#include <string>

/**
 * @brief Enum for boundary behavior
 */
enum class BoundaryBehavior {
    Periodic,
    Reflective,
    Outflow
};

/**
 * @brief Converts boundary behavior to string
 *
 * @param boundaryBehavior
 * @return String representation of boundary behavior
 */
std::string boundaryBehaviorToString(BoundaryBehavior boundaryBehavior);

/**
 * @brief Converts string to boundary behavior
 *
 * @param boundaryBehavior
 * @return BoundaryBehavior represemted by a string
 */
BoundaryBehavior stringToBoundaryBehavior(const std::string &boundaryBehavior);