//
// Created by alex on 29.10.2022.
//
#pragma once

#include <vector>
#include <string>
#include "io/input/sim_input/types.h"

namespace io::input {
    /**
     * Exit the application with error, print usage and help, append @param msg.
     * */
    [[noreturn]] void exitFormatError(const std::string &&msg);

    /**
     * Print help.
     * */
    void printHelp();

    /**
     * Structures CLI args and allows easy access.
     * */
    class CLIArgsParser {
    private:
        std::vector<std::string> args{};

    public:
        /**
         * Will skip argsv[0] and not store it.
         * @param argc argsv size
         * @param argsv all cli arguments
         * */
        CLIArgsParser(int argc, char *argsv[]);

        ~CLIArgsParser() = default;

        /**
         * Checks if
         * @param op exists in the stored arguments and
         * @returns true iff that is the case.
         * */
        bool optionExists(const std::string_view &op);

        /**
         * Checks if the provided option @param op exists by calling CLIArgsParser::optionExists and checks if this
         * option has an argument.
         * @returns true iff both are true.
         * */
        bool optionArgExists(const std::string_view &op);

        /**
         * Retrieve the argument of the specified option @param op and @returns is as std::string.
         * */
        std::string getOptionArg(const std::string_view &op);

        /**
         * Places all args that are not prepended by an option specifier with -, such as -dt or -et, into @param buffer.
         * */
        void getInputPaths(std::vector<std::string> &buffer);

        /**
         * Uses the arg registry in CLIArgs.h to extract all arguments. Results will also be stored in io::input::cli_arg_map.
         * */
        void parseArgs();

        /**
         * @brief Returns the valid loader type, which was either defined by cli arg or is the default value.
         * Must call parseArgs() prior.
         * */
        type getLoader();
    };

} // io::input

