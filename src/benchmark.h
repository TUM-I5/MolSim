//
// Created by alex on 26.11.2022.
//

#pragma once

#include "io/input/cli/CLIArgs.h"

int runBenchmark(double dt, double et, double st, double sig, double eps, std::vector<std::string>& inputFiles);

/**
 *
 * // handle benchmark request
    if (parser.optionArgExists("-bench")) {
        int iterations;
        if (!parser.optionArgExists("-i")) iterations = default_bench_iterations;
        else {
            std::string tmp = parser.getOptionArg("-i");
            iterations = std::stoi(tmp);
        }

        std::string type = parser.getOptionArg("-bench");
        if (type == "default") {
            // get max body size
            int maxBodySize;
            if (parser.optionArgExists("-bMax")) {
                std::string tmp = parser.getOptionArg("-bMax");
                maxBodySize = std::stoi(tmp);
            } else maxBodySize = default_bench_maxBody;

            // generate 2 bodies in varying sizes
            struct Body b0 { Shape::cuboid, {-1.1, 0, 0}, {10, 10, 1}, 0.1, 0.1, {0.001, 0, 0} };
            struct Body b1 { Shape::cuboid, { 0.1, 0, 0}, {10, 10, 1}, 0.1, 0.1, {0.001, 0, 0} };
            std::list<Particle> buffer_tmp;
            std::vector<Particle> buffer;
            for(int bSize { 1 }; bSize <= maxBodySize; bSize *= 2) {
                b0.dimensions[2] = bSize;
                b1.dimensions[2] = bSize;

                ParticleGenerator::generateCuboid(b0, 0.1, buffer_tmp, 3);
                ParticleGenerator::generateCuboid(b1, 0.1, buffer_tmp, 3);
                for(const auto& p : buffer_tmp) buffer.push_back(p);
                sim::Simulation<calcF, calcX, calcV> simulation {st, et, dt, eps, sig, outputFolder, outputBaseName};
                simulation.runBenchmark(iterations, "default", buffer);

                buffer_tmp.clear();
                buffer.clear();
            }

        }
        else if (type == "file") {
            std::vector<Particle> buffer;
            for(const auto& file : inputFiles) {
                auto iow = io::IOWrapper<io::input::BodyReader>(file.c_str());
                iow.reload();
                iow.getParticles(buffer);
                if (!epsOverride) eps = iow.getEpsilon();
                if (!sigOverride) sig = iow.getSigma();

                sim::Simulation<calcF, calcX, calcV> simulation {st, et, dt, eps, sig, outputFolder, outputBaseName};
                simulation.runBenchmark(iterations, file, buffer);
                buffer.clear();
            }
        }
        else io::input::exitFormatError(type + ": is an unknown benchmark input type!");
        return 0;
    }
*/