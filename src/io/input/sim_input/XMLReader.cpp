//
// Created by jan on 11/29/22.
//

#include "XMLReader.h"

namespace io::input {
    XMLReader::XMLReader() = default;

    XMLReader::~XMLReader() = default;

    void XMLReader::readFile(const char *filename, std::list<Particle> &particles,
                             std::unordered_map<io::input::names, std::string> &) {
        
    }
}