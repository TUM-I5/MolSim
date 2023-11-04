//
// Created by Alp Kaan Aksu on 04.11.23.
//

#include "JSONReader.h"

#include <fstream>

json JSONReader::readFile(const std::string &filename) {
    std::ifstream ifs(filename);
    json j = json::parse(ifs);

    return j;
}