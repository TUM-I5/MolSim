#include "FileUtils.h"

std::string remove_whitespace(const std::string& str) {
    std::string result = str;
    result.erase(std::remove_if(result.begin(), result.end(), [](unsigned char x) { return std::isspace(x); }), result.end());
    return result;
}