#include "FormatTime.h"

#include "io/logger/Logger.h"

std::string format_seconds_eta(int total_seconds) {
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    return fmt::format("{:02d}:{:02d}:{:02d}", hours, minutes, seconds);
}

std::string format_seconds_total(double total_seconds) {
    int hours = static_cast<int>(total_seconds) / 3600;
    int minutes = (static_cast<int>(total_seconds) % 3600) / 60;
    int seconds = static_cast<int>(total_seconds) % 60;
    int millis = (total_seconds - static_cast<int>(total_seconds)) * 1000;

    return fmt::format("{}h {}m {}s {}ms", hours, minutes, seconds, millis);
}