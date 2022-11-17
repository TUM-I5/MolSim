
#pragma once

#include <string>
#include <regex>
#include <fstream>

namespace Input{
inline std::string ftrim(std::string s){
    return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

inline std::string trim(std::string s){
    s = std::regex_replace(s, std::regex("\\s+"), std::string(""));
    return ftrim(s);
}

inline std::string trim(std::string s, std::string to_be_replaced){
    return std::regex_replace(s, std::regex(to_be_replaced), std::string(""));
}

inline bool isDouble(std::string s){
    return std::regex_match(s, std::regex("[0-9]*([\\.][0-9]+)?")); 
}

inline bool isValidLevel(std::string s){
    return std::regex_match(s, std::regex("[0-1]")); 
}

inline bool isInt(std::string s){
    return std::regex_match(s, std::regex("[0-9]+")); 
}

inline bool isValidFilename(std::string s){
    std::ifstream test(trim(trim(s, "\"")));
    return !test; 
}
};