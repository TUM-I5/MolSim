#include "CubFileReader.h"

#include <fstream>
#include <iostream>
#include <limits>

#include "io/logger/Logger.h"
#include "io/particle_spawners/CuboidSpawner.h"

void CubFileReader::readFile(const std::string& filepath, DirectSumContainer& particleContainer) const {
    FileLineReader input_file(filepath);

    if (!input_file.is_open()) {
        Logger::logger->error("Error: could not open file '{}'.", filepath);
        throw FileFormatException();
    }

    while (!input_file.eof()) {
        while (input_file.peek() == '#' || input_file.peek() == '\n') {
            input_file.skipLine();
        }

        input_file.nextLine();
        double x, y, z;
        input_file.getLineStream() >> x >> y >> z >> std::ws;
        checkAndReportInvalidEntry(input_file, "<double> <double> <double>");
        std::array<double, 3> lower_left_front_corner{x, y, z};

        input_file.nextLine();
        int nx, ny, nz;
        input_file.getLineStream() >> nx >> ny >> nz >> std::ws;
        checkAndReportInvalidEntry(input_file, "<int> <int> <int>");
        std::array<int, 3> grid_dimensions{nx, ny, nz};

        input_file.nextLine();
        double grid_spacing;
        input_file.getLineStream() >> grid_spacing >> std::ws;
        checkAndReportInvalidEntry(input_file, "<double>");

        input_file.nextLine();
        double mass;
        input_file.getLineStream() >> mass >> std::ws;
        checkAndReportInvalidEntry(input_file, "<double>");

        input_file.nextLine();
        double vx, vy, vz;
        input_file.getLineStream() >> vx >> vy >> vz >> std::ws;
        checkAndReportInvalidEntry(input_file, "<double> <double> <double>");
        std::array<double, 3> initial_velocity{vx, vy, vz};

        input_file.nextLine();
        double mean_wobbling_velocity;
        input_file.getLineStream() >> mean_wobbling_velocity >> std::ws;
        checkAndReportInvalidEntry(input_file, "<double>");

        input_file.nextLine();
        int type;
        input_file.getLineStream() >> type >> std::ws;
        checkAndReportInvalidEntry(input_file, "<int>");

        auto spawner = CuboidSpawner(lower_left_front_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type);

        particleContainer.reserve(particleContainer.size() + nx * ny * nz);
        spawner.spawnParticles(particleContainer);
    }
}

bool checkInvalid(std::stringstream& curr_line_stream) {
    return curr_line_stream.fail() || (curr_line_stream.peek() != '#' && curr_line_stream.peek() != EOF);
}

void CubFileReader::checkAndReportInvalidEntry(FileLineReader& input_file, const std::string& expected_format) const {
    if (input_file.getLineStream().fail()) {
        Logger::logger->error(
            "Invalid entry in file '{}' on line {}.\n"
            "\t Expected format: '{}'\n"
            "\t Got: '{}'",
            input_file.getFilePath(), input_file.getLineNumber(), expected_format, input_file.getLine());

        throw FileFormatException();
    }

    if (input_file.getLineStream().peek() != '#' && input_file.getLineStream().peek() != EOF) {
        Logger::logger->error(
            "Invalid entry in file '{}' on line {}.\n"
            "\t Comments must start with: '#', but got: '{}'\n"
            "\t Content of line: '{}'",
            input_file.getFilePath(), input_file.getLineNumber(), static_cast<char>(input_file.getLineStream().peek()),
            input_file.getLine());

        if (input_file.getLine().find('#') != std::string::npos) {
            Logger::logger->error("Make sure that comments start after the arguments in the line.");
        }

        throw FileFormatException();
    }
}