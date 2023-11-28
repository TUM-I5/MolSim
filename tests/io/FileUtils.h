#pragma once

#include <algorithm>
#include <cctype>
#include <string>

#define MULTILINE(...) #__VA_ARGS__

std::string remove_whitespace(const std::string& str);