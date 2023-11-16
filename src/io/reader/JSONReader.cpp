//
// Created by Alp Kaan Aksu on 04.11.23.
//

#include "JSONReader.h"

#include <boost/filesystem.hpp>
#include "spdlog/spdlog.h"

#include <fstream>

json JSONReader::readFile(const std::string &filename) {
    // check if file at filename exists

    if (!boost::filesystem::exists(filename)) {
        spdlog::error("File " + filename + " does not exist.");
        exit(-1);
    }

    std::ifstream ifs(filename);
    json j = json::parse(ifs);

    return j;
}