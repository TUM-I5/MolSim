#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "particleModel/updating/CellCalculator.h"
#include "Simulation.h"
#include "outputWriter/XYZWriter.h"
#include <spdlog/spdlog.h>
#include <iostream>

void runSimulation(CellContainer &container, CellCalculator calculator, const double end_time,
                   const double delta_t, const size_t write_frequency, const int thermostats_frequency, bool performance_measurement) {

    outputWriter::VTKWriter writer;
    auto logger = spdlog::get("logger");

    std::chrono::high_resolution_clock::time_point perf_time_start, perf_time_end;

    double current_time = 0;
    int iteration = 0;

    std::string progressBar;
    size_t barWidth, pos;

    SPDLOG_INFO("Initalizing Simulation");
    calculator.initializeFX();

    SPDLOG_LOGGER_DEBUG(logger, "Particles in the simulation:");
    SPDLOG_LOGGER_DEBUG(logger, container.to_string());
    logger->flush();

    //size_t before_size = container.size();

    // for this loop, we assume: current x, current f and current v are known
    if (performance_measurement)
        perf_time_start = std::chrono::high_resolution_clock::now();

    while (current_time < end_time) {
        SPDLOG_TRACE(std::to_string(current_time));

        SPDLOG_TRACE("Doing a Iteration with CellCalculator");
        calculator.applyBoundaries();
        //new order to directly calculate F~ & V & X for each cell
        calculator.calculateLinkedCellF();
        calculator.calculateWithinFVX();
        
        iteration++;

        if (iteration % write_frequency == 0 && !performance_measurement) {
            writer.initializeOutput(container.size());
            container.plotParticles(writer);
            writer.writeFile("out", iteration);
        }

        if (iteration % thermostats_frequency == 0 && thermostats_frequency != -1) {
            calculator.applyThermostats();
        }

        /// loading bar
        if (iteration % write_frequency * 5 == 0 && !performance_measurement) {
            barWidth = 50;
            pos = static_cast<size_t>(barWidth * (current_time / end_time));
            progressBar = "[" + std::string(pos, '=') + '>'
                          + std::string(barWidth - pos, ' ') + "] "
                          + std::to_string(int((current_time / end_time) * 100.0)) + "%\r";

            spdlog::info(progressBar);
        }

        current_time += delta_t;
    }
    if (performance_measurement) {
        perf_time_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> perf_duration = perf_time_end - perf_time_start;
        std::cout << "The Computation took: " << perf_duration.count() << " seconds" << std::endl;
    }
    
    //std::cout << "before: " << before_size << std::endl;
    //std::cout << "after: " << container.size() << std::endl;
    spdlog::info("[" + std::string(pos, '=') + ">] 100%\r");
    SPDLOG_INFO("output written. Terminating...\r");
}


void plotParticles(CellContainer &container, int iteration) {
  std::string out_name("MD_vtk");
  outputWriter::XYZWriter writer;
  writer.plotParticles(container, out_name, iteration);
}