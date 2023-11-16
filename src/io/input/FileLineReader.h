#pragma once

#include <fstream>
#include <sstream>
#include <string>

class FileLineReader {
    const std::string filepath;
    std::ifstream file_stream;

    std::string curr_line;
    std::stringstream curr_line_stream;

    int current_line_number = 0;

   public:
    /**
     * @brief Construct a new FileLineReader object
     * @param filepath Name of the file to read
     */
    FileLineReader(const std::string& filepath);

    /**
     * @brief Skip the next line in the file, does and update the value of `getLineNumber()`, the line content however is not stored
     */
    void skipLine();

    /**
     * @brief Read the next line from the file and update the values of `getLine()`  getLineStream()` and `getLineNumber()`
     */
    void nextLine();

    /**
     * @brief Return the content of the last line read from the file
     * @return The content of the last line read from the file
     */
    const std::string& getLine() const;

    /**
     * @brief Reference to the remaining unprocessed part of the last line read from the file
     * @return The remaining unprocessed part of the last line read from the file
     */
    std::stringstream& getLineStream();

    /**
     * @brief Check if the file is open, wrapper for `std::ifstream::is_open()`
     * @return True if the file is open, false otherwise
     */
    bool is_open() const;

    /**
     * @brief Check if the end of the file has been reached, wrapper for `std::ifstream::eof()`
     * @return True if the end of the file has been reached, false otherwise
     */
    bool eof() const;

    /**
     * @brief Get the next character from the file without extracting it, wrapper for `std::ifstream::peek()`
     * @return The next character from the file
     */
    int peek();

    /**
     * @brief Get the line number of the last line returned by `getline()`
     * @return The line number of the last line returned by `getline()`
     */
    int getLineNumber() const;

    /**
     * @brief Get the path of the file
     * @return The path of the file
     */
    const std::string& getFilePath() const;
};