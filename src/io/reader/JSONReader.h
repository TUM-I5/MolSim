//
// Created by Alp Kaan Aksu on 04.11.23.
//

#pragma once

#include "Simulation.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
 * JSONReader class
 * Reads a JSON file and returns a json object
 */
class JSONReader {
public:
    JSONReader();
    virtual ~JSONReader();

    /**
   * @brief Read particles from a JSON file into a ParticleContainer
   *
   * @param particles
   * @param filename
   */
    static json readFile(const std::string &filename);
};
