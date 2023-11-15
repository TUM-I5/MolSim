#include "FileLineReader.h"

#include "io/logger/Logger.h"

FileLineReader::FileLineReader(const std::string& filepath) : filepath(filepath), file_stream(filepath) {}

void FileLineReader::skipLine() {
    std::string buffer;
    std::getline(file_stream, buffer);
    current_line_number++;
}

void FileLineReader::nextLine() {
    std::getline(file_stream, curr_line);
    current_line_number++;
    curr_line_stream = std::stringstream(curr_line);
}

const std::string& FileLineReader::getLine() const {
    return curr_line;
}

std::stringstream& FileLineReader::getLineStream() {
    return curr_line_stream;
}

bool FileLineReader::is_open() const {
    return file_stream.is_open();
}

bool FileLineReader::eof() const {
    return file_stream.eof();
}

int FileLineReader::peek() {
    return file_stream.peek();
}

int FileLineReader::getLineNumber() const {
    return current_line_number;
}

const std::string& FileLineReader::getFilePath() const {
    return filepath;
}