//
// Created by alex on 07.12.22.
//
#include "types.h"
#include "io/input/cli/CLIArgsParser.h"

namespace io::input {
    std::unordered_map<std::string, type> type_map = {{"xml" , XML},
                                                      {"file", FILE},
                                                      {"body", BODY}};

    type stot(const std::string &str) {
        auto lowercase = [](std::string &str) {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
        };
        std::string key = str;
        lowercase(key);
        if (!type_map.contains(key)) io::input::exitFormatError("loader type unknown!");
        else return type_map[key];
    }


} //io::input