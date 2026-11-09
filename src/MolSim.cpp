
#include <CLI11/CLI11.hpp>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <string>

#include "Simulation.h"
#include "inputReader/FileReader.h"
#include "outputWriter/OutputWriter.h"
#include "utils/Logging.h"
#include "utils/ProgressBar.h"

int main(int argc, char *argsv[]) {
    // Set by CLI11
    double delta_t;
    double end_time;
    double start_time;

    std::string file_path;

    const std::map<std::string, output_writer::OutputFormat> format_names{
        {"xyz", output_writer::OutputFormat::kXyz},
#ifdef ENABLE_VTK_OUTPUT
        {"vtk", output_writer::OutputFormat::kVtk},
        {"vtka", output_writer::OutputFormat::kVtkAscii},
#endif
    };
    std::string format_list;
    for (const auto &name : format_names | std::views::keys) {
        if (!format_list.empty()) {
            format_list += ',';
        }
        format_list += name;
    }

    std::vector<std::unique_ptr<output_writer::OutputWriter>> writers;

    // parse command line inputs with CLI 11
    CLI::App app("MD simulation");

    // optional arguments for delta T and end time, provided with default values
    app.add_option("file", file_path, "Input File")->required()->check(CLI::ExistingFile);

    app.add_option("--dt", delta_t, "Time step per iteration")
        ->default_val(0.014)
        ->check(CLI::PositiveNumber);
    app.add_option("--end", end_time, "End Time of the sim")->default_val(1000.0);
    app.add_option("--start", start_time, "Start time of the sim")->default_val(0.0);
    std::vector<output_writer::OutputFormat> output_formats;
    app.add_option("--format", output_formats, "Output format")
        ->transform(CLI::CheckedTransformer(format_names, CLI::ignore_case))
        ->delimiter(',')
        ->option_text(format_list);
    int write_every;
    app.add_option("--write_every", write_every, "Write output every n iterations")
        ->default_val(10)
        ->check(CLI::PositiveNumber);
    std::filesystem::path out_dir;
    app.add_option("-o,--output", out_dir, "Output directory")->default_val("output");
    const std::map<std::string, spdlog::level::level_enum> log_levels{
        {"trace", spdlog::level::trace}, {"debug", spdlog::level::debug},
        {"info", spdlog::level::info},   {"warn", spdlog::level::warn},
        {"error", spdlog::level::err},   {"off", spdlog::level::off},
    };
    auto log_lvl = spdlog::level::info;
    app.add_option("--log-level", log_lvl, "Log level")
        ->transform(CLI::CheckedTransformer(log_levels, CLI::ignore_case));

    app.callback([&] {
        if (!std::isfinite(start_time) || !std::isfinite(end_time) || !std::isfinite(delta_t)) {
            throw CLI::ValidationError("--start/--end/--dt", "must be finite numbers");
        }
        if (end_time <= start_time) {
            throw CLI::ValidationError("--end", "must be greater than the start time");
        }
    });

    app.footer(
        "Example:\n"
        "./MolSim input/eingabe-sonne.txt --dt 0.014 --end 1000");
    app.failure_message(CLI::FailureMessage::help);

    // parse with library functions
    CLI11_PARSE(app, argc, argsv);

    logging::InitLogging(log_lvl);

    SPDLOG_INFO("MolSim started");

    ParticleContainer particles;
    try {
        auto reader = file_reader::FileReader::MakeReader(file_path);
        reader->Read(particles);
    } catch (const std::exception &e) {
        SPDLOG_ERROR("Could not read input: {}", e.what());
        return 1;
    }

    if (particles.size() == 0) {
        SPDLOG_WARN("No particles to simulate");
    }

    SPDLOG_INFO("Simulation with {} particles.", particles.size());

    // Add default writers
    if (output_formats.empty()) {
        output_formats.push_back(output_writer::OutputFormat::kXyz);
    }

    // Create Writers
    const std::string out_name("MD_iteration");
    for (const std::set unique_formats(output_formats.begin(), output_formats.end());
         const auto format : unique_formats) {
        writers.push_back(output_writer::OutputWriter::MakeWriter(format, out_dir, out_name));
    }

    ProgressBar bar{start_time, end_time};

    const auto post_iter = [&writers, write_every, &bar](const ParticleContainer &p,
                                                         const int iteration, const double time) {
        if (iteration % write_every == 0) {
            output_writer::OutputWriter::PlotParticles(p, writers, iteration);
        }
        bar.Update(time);
    };

    simulation::Simulation sim{std::move(particles), {start_time, end_time, delta_t}};

    sim.Run(post_iter);

    SPDLOG_INFO("Simulation finished, output written to {}, Terminating ...", out_dir.string());
    return 0;
}
