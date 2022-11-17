/*
 * InputReader.cpp
 *
 *  Created on: 11.10.2022
 *      Author: wohlrapp
 */

#include "InputReader.h"

InputReader::InputReader(){
    _logicLogger = spdlog::get("input_logger");
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("InputReader generated!");
}

InputReader::~InputReader(){
    _memoryLogger->info("InputReader destructed!");
}

const std::shared_ptr<spdlog::logger> InputReader::getLogicLogger() const { return _logicLogger; }
