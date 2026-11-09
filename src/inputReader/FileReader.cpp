#include "FileReader.h"

#include <memory>

#include "JsonReader.h"
#include "TxtReader.h"

namespace file_reader {

std::unique_ptr<FileReader> FileReader::MakeReader(const std::string file_path) {
    const auto extension = std::filesystem::path(file_path).extension().string();

    if (extension == ".txt") {
        return std::make_unique<TxtReader>(file_path);
    }
    if (extension == ".json") {
        return std::make_unique<JsonReader>(file_path);
    }

    throw std::invalid_argument("Unsupported input file type: " + extension);
}

}  // namespace file_reader
