/*
 *  Input.h
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#pragma once

#include <string>
#include <regex>
#include <fstream>

/**
 * @brief namespace that includes functions to validate and sanitize the input
 */
namespace Input
{

    /**
     * @brief trims of the whitespaces at the front
     * @param s the string from which whitespace is removed
     * @returns the string without the whitespace at the front
     */
    inline std::string ftrim(std::string s)
    {
        return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
    }

    /**
     * @brief trims of the whitespaces
     * @param s the string from which whitespace is removed
     * @returns the string without the whitespace
     */
    inline std::string trim(std::string s)
    {
        s = std::regex_replace(s, std::regex("\\s+"), std::string(""));
        return ftrim(s);
    }

    /**
     * @brief replaces a sequence in a string with ""
     * @param s the string from which the sequence is removed
     * @param to_be_replaced the sequence that should be replaced
     * @returns the string without the sequence
     */
    inline std::string trim(std::string s, std::string to_be_replaced)
    {
        return std::regex_replace(s, std::regex(to_be_replaced), std::string(""));
    }

    /**
     * @brief check if string is a double
     * @param s the string which is checked
     * @returns true if string is a double, false otherwise
     */
    inline bool isDouble(std::string s)
    {
        return std::regex_match(s, std::regex("[0-9]*([\\.][0-9]+)?"));
    }

    /**
     * @brief check if string is a valid log level
     * @param s the string which is checked
     * @returns true if string is a valid log level, false otherwise
     */
    inline bool isValidLogLevel(std::string s)
    {
        return std::regex_match(s, std::regex("[oecwidt]"));
    }

    /**
     * @brief check if string is a valid log mode
     * @param s the string which is checked
     * @returns true if string is a valid log mode, false otherwise
     */
    inline bool isValidLogMode(std::string s)
    {
        return std::regex_match(s, std::regex("[cf]"));
    }

    /**
     * @brief check if string is an int
     * @param s the string which is checked
     * @returns true if string is an int, false otherwise
     */
    inline bool isInt(std::string s)
    {
        return std::regex_match(s, std::regex("[0-9]+"));
    }

    inline bool isXML(std::string s){
        if(s.length() < 5)
            return false; 
        if(s.at(s.length() - 1) != 'l')
            return false; 
        if(s.at(s.length() - 2) != 'm')
            return false; 
        if(s.at(s.length() - 3) != 'x')
            return false; 
        if(s.at(s.length() - 4) != '.')
            return false; 
         
        return true; 
    }
}