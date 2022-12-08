//
// Created by alex on 30.11.22.
//

#include <unordered_map>
#include "types.h"
#include "io/input/cli/CLIArgsParser.h"

namespace sim::physics::bounds {
    std::unordered_map<std::string, type> type_map = {{"outflow",    outflow},
                                                      {"reflecting", reflecting},
                                                      {"periodic", periodic}};

    type stot(const std::string &str) {
        auto lowercase = [](std::string &str) {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
        };
        std::string key = str;
        lowercase(key);
        if (!type_map.contains(key)) io::input::exitFormatError("boundary type unknown!");
        else return type_map[key];
    }

} // sim::physics::bounds