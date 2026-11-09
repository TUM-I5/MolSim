#include "ProgressBar.h"

#include <algorithm>
#include <iostream>
#include <string>

void ProgressBar::Update(const double time) {
    if (end_time_ <= start_time_ || finished_) {
        return;
    }

    const double progress = (time - start_time_) / (end_time_ - start_time_) * 100.0;
    const int percent = static_cast<int>(std::clamp(progress, 0.0, 100.0));

    // Redraw only when the displayed percentage changes.
    if (percent == last_percent_) {
        return;
    }
    last_percent_ = percent;

    const int filled = percent * width_ / 100;

    std::string bar(width_, ' ');
    for (int i = 0; i < filled; ++i) {
        bar[i] = '=';
    }
    if (filled < width_) {
        bar[filled] = '>';
    }

    std::cerr << '\r' << '[' << bar << "] " << percent << '%' << std::flush;

    if (percent == 100) {
        std::cerr << '\n';
        finished_ = true;
    }
}
