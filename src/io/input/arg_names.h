//
// Created by alex on 25.11.2022.
//

#pragma once

#include <string_view>

/**
 * Defines names in the map for arguments in the input loader
 * */
namespace io::input::names {
    constexpr std::string_view sigma {"sig"};
    constexpr std::string_view epsilon {"eps"};
    constexpr std::string_view brown {"brown"};
    constexpr std::string_view dimensions {"dims"};
} // io::input::names