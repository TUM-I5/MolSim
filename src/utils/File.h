/*
 *  File.h
 *
 *  Created on: 14.12.2022
 *      Author: wohlrapp
 */

#pragma once

#include <string>
#include <ctime>
#include <iostream>

namespace File
{
    /**
     * @brief returns the current date time as a string
     * @returns a string of the current date time
    */
    inline std::string getDateTime()
    {
        std::time_t time = std::time(0);
        std::tm *now = std::localtime(&time);
        std::ostringstream prefix;
        prefix << (now->tm_year + 1900) << '-'
               << (now->tm_mon + 1) << '-'
               << now->tm_mday << '-'
               << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << '_';

        return prefix.str();
    }
}