//
// Created by Alp Kaan Aksu on 16.11.23.
//

#include "Writer.h"

#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>

namespace fs = boost::filesystem;

void outputWriter::prepareOutputFolder(const std::string &path) {
    // Check if given path exists
    if (fs::exists(path)) {
        // Path exists: remove the folder and create it again
        spdlog::info("Folder already exists: " + path);
        spdlog::info("Removing the folder: " + path);
        fs::remove_all(path);

        if (!fs::create_directory(path)) {
            spdlog::error("Failed to create the directory: " + path);
        } else {
            spdlog::info("Folder created successfully: " + path);
        }
    } else {
        // Path doesn't exist: create the directory
        if (!fs::create_directory(path)) {
            spdlog::error("Failed to create the directory: " + path);
        } else {
            spdlog::info("Folder created successfully: " + path);
        }
    }
}