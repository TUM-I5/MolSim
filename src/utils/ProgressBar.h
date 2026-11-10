#pragma once

class ProgressBar {
    bool finished_ = false;
    int last_percent_ = -1;
    double start_time_;
    double end_time_;
    int width_;

   public:
    /**
     * @brief Progress bar to see how far the Simulation is
     *
     * @param start_time Start of the simulation
     * @param end_time End of the simulation
     * @param width Width of the bar in chars
     */
    ProgressBar(double start_time, double end_time, int width = 50)
        : start_time_(start_time), end_time_(end_time), width_(width) {}

    /**
     * @brief Update the displayed progress bar
     *
     * Prints to `std::cerr`
     *
     * @param time Current time of the simulation
     */
    void Update(double time);
};
