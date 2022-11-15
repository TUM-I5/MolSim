//
// Created by alex on 29.10.2022.
//
#pragma once

#include <vector>
#include <string>

namespace cli {
    /**
     * Exit the application with error, print usage and help, append @param msg.
     * */
    void exitFormatError(const std::string &&msg);

    /**
     * Print help.
     * */
     void printHelp();

    /**
     * Structures CLI args and allows easy access.
     * */
    class ArgsParser {
    private:
        std::vector<std::string> args{};

    public:
        /**
         * Will skip argsv[0] and not store it.
         * @param argc argsv size
         * @param argsv all cli arguments
         * */
        ArgsParser(int argc, char *argsv[]);

        ~ArgsParser() = default;

        /**
         * Checks if
         * @param op exists in the stored arguments and
         * @returns true iff that is the case.
         * */
        bool optionExists(const std::string &op);

        /**
         * Checks if the provided option @param op exists by calling ArgsParser::optionExists and checks if this
         * option has an argument.
         * @returns true iff both are true.
         * */
        bool optionArgExists(const std::string &op);

        /**
         * Retrieve the argument of the specified option @param op and @returns is as std::string.
         * */
        std::string getOptionArg(const std::string &op);

        /**
         * Places all args that are not prepended by an option specifier with -, such as -dt or -et, into @param buffer.
         * */
        void getInputPaths(std::vector<std::string> &buffer);
    };

} // cli
