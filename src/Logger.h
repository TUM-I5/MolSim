//
// Created by sophy on 13.11.2023.
//

#ifndef PSEMOLDYN_GROUPE_LOGGER_H
#define PSEMOLDYN_GROUPE_LOGGER_H

// Logger.h
#pragma once

#include <spdlog/spdlog.h>

class Logger {
public:
    static void init();

    static std::shared_ptr<spdlog::logger>& getCoreLogger();
    static std::shared_ptr<spdlog::logger>& getClientLogger();

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};


#endif //PSEMOLDYN_GROUPE_LOGGER_H
