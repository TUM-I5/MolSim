//
// Created by Alp Kaan Aksu on 09.12.23.
//

#include "JSONWriter.h"

#include <fstream>
#include <iostream>

#include "spdlog/spdlog.h"

JSONWriter::JSONWriter() = default;

JSONWriter::~JSONWriter() = default;

void JSONWriter::writeFile(const nlohmann::ordered_json &json, const std::string &filename) {
    std::ofstream ofs(filename);
    ofs << json.dump(4) << std::endl;
    ofs.close();
}