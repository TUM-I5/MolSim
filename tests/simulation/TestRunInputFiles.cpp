#include <gtest/gtest.h>

#include <filesystem>
#include <iostream>
#include <memory>

#include "data/FileLoader.h"
#include "io/input/FileInputHandler.h"
#include "particles/containers/ParticleContainer.h"
#include "physics/ForceSource.h"
#include "physics/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "simulation/SimulationUtils.h"
#include "utils/ArrayUtils.h"

auto load_all_input_files() {
    std::vector<std::string> input_files;
    for (const auto& entry : std::filesystem::directory_iterator(FileLoader::get_test_data_dir() + "/../../input")) {
        // check for valid extension
        auto supported_extensions = FileInputHandler::get_supported_input_file_extensions();

        if (supported_extensions.find(entry.path().extension()) == supported_extensions.end()) {
            continue;
        }

        input_files.push_back(entry.path());
    }
    return input_files;
}

/*
 * Test that all input files can be read and simulated
 */
TEST(SimulationRunner, EnsureBackwardsCompatibilityForAllInputFiles) {
    Logger::logger->set_level(spdlog::level::warn);

    std::set<std::string> tested_extensions = {};

    for (const auto& input_file : load_all_input_files()) {
        std::cout << "Testing input file: " << input_file << std::endl;

        tested_extensions.insert(input_file.substr(input_file.find_last_of('.')));

        // Create pointer for particle container
        std::unique_ptr<ParticleContainer> initial_particles;

        // Parse input file
        SimulationParams params_xml = FileInputHandler::readFile(input_file, initial_particles);

        EXPECT_GT(initial_particles->size(), 0);

        // Create all force sources acting on the particles
        std::vector<std::unique_ptr<ForceSource>> forces;
        forces.push_back(std::make_unique<LennardJonesForce>());

        params_xml.end_time = 1;
        params_xml.delta_t = 0.01;
        params_xml.output_format = FileOutputHandler::OutputFormat::NONE;

        // Initialize simulation
        Simulation simulation{initial_particles, forces, params_xml};

        // Run simulation
        auto res = simulation.runSimulation();

        EXPECT_GT(res.total_iterations, 0);
    }

    // Check that all supported extensions have actually been tested
    for (const auto& extension : FileInputHandler::get_supported_input_file_extensions()) {
        EXPECT_TRUE(tested_extensions.find(extension) != tested_extensions.end());
    }
}