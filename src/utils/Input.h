
#pragma once

#include <string>
#include <regex>

namespace Input {

std::string ftrim(std::string s){
    return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

std::string trim(std::string s){
    s = std::regex_replace(s, std::regex("\\s+"), std::string(""));
    return ftrim(s);
}

std::string trim(std::string s, std::string to_be_replaced){
    return std::regex_replace(s, std::regex(to_be_replaced), std::string(""));
}

bool isDouble(std::string s){
    return std::regex_match(s, std::regex("[0-9]*([\\.][0-9]+)?")); 
}

bool isValidLevel(std::string s){
    return std::regex_match(s, std::regex("[0-1]")); 
}

bool isInt(std::string s){
    return std::regex_match(s, std::regex("[0-9]+")); 
}
}