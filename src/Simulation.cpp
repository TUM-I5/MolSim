#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "particleModel/updating/CellCalculator.h"
#include "Simulation.h"
#include "outputWriter/XYZWriter.h"
#include <spdlog/spdlog.h>
#include <iostream>

void runSimulation(CellContainer &container, CellCalculator& calculator, const double end_time,
                   const double delta_t, const size_t write_frequency, std::optional<int> thermostats_frequency, bool performance_measurement) {

    outputWriter::VTKWriter writer;
    auto logger = spdlog::get("logger");

    std::chrono::high_resolution_clock::time_point perf_time_start, perf_time_end;

    double current_time = 0;
    int iteration = 0;

    std::string progressBar;
    size_t barWidth, pos;

    SPDLOG_INFO("Starting Simulation");
    calculator.initializeFX();

    SPDLOG_LOGGER_DEBUG(logger, "Particles in the simulation:");
    SPDLOG_LOGGER_DEBUG(logger, container.to_string());
    logger->flush();

    if (performance_measurement)
        perf_time_start = std::chrono::high_resolution_clock::now();

    while (current_time < end_time) {
        SPDLOG_TRACE(std::to_string(current_time));

        SPDLOG_TRACE("Doing a Iteration with CellCalculator");
        calculator.applyReflectiveBoundaries();
        //new order to directly calculate F~ & V & X for each cell
        calculator.calculateLinkedCellF();
        calculator.calculateWithinFVX();

        iteration++;

        if (!performance_measurement && iteration % write_frequency == 0) {
            writer.initializeOutput(container.size());
            container.plotParticles(writer);
            writer.writeFile("out", iteration);
        }

        //thermostats_frequency.has_value() will be evaluated first
        if (thermostats_frequency.has_value() &&  iteration % thermostats_frequency.value() == 0) {
            calculator.applyThermostats();
        }

        /// loading bar
        static int loading_factor = std::max(write_frequency * 5.0, std::ceil(end_time / (delta_t * 100)));

        if (!performance_measurement && iteration % loading_factor == 0) {
            barWidth = 50;
            pos = static_cast<size_t>(barWidth * (current_time / end_time));
            progressBar = "[" + std::string(pos, '=') + '>'
                          + std::string(barWidth - pos, ' ') + "] "
                          + std::to_string(int((current_time / end_time) * 100.0)) + "%\r";

            spdlog::info(progressBar);
        }

        current_time += delta_t;
    }

    auto runtimeDuration=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - perf_time_start).count();

    double mups = static_cast<double>(container.size()) * iteration /
                  (static_cast<double>(runtimeDuration) / 1000);
    std::cout
            << "Calculation time in milliseconds: "
            << runtimeDuration
            << std::endl;
    if (mups > 1000000) {
        std::cout << "MMUPS/s: " << mups / 1000000 << std::endl;
    } else {
        std::cout << "MUPS/s: " << mups << std::endl;
    }

    spdlog::info("[" + std::string(pos, '=') + ">] 100%\r");
    SPDLOG_INFO("output written. Terminating...\r");
}


void plotParticles(CellContainer &container, int iteration) {
  std::string out_name("MD_vtk");
  outputWriter::XYZWriter writer;
  writer.plotParticles(container, out_name, iteration);
}