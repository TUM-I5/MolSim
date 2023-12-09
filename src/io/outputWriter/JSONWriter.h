//
// Created by Alp Kaan Aksu on 09.12.23.
//

#pragma once

#include <nlohmann/json.hpp>

class JSONWriter {
public:
    JSONWriter();
    virtual ~JSONWriter();

    /**
     * @brief Write particles to a JSON file
     *
     * @param particles
     * @param filename
     */
    static void writeFile(const nlohmann::ordered_json &json, const std::string &filename);
};
