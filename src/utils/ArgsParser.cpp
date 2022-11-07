//
// Created by alex on 29.10.2022.
//

#include "ArgsParser.h"

#include <algorithm>
#include <iostream>

namespace cli {
    void exitFormatError(const std::string &&msg) {
        printHelp();
        std::cout << msg << std::endl;
        exit(EXIT_FAILURE);
    }

    void printHelp() {
        std::cout << "Welcome to MolSim Help" << std::endl
                  << "Usage:" << std::endl
                  << "MolSim <input-files> <options>" << std::endl;

        std::cout << "Options:" << std::endl
                  << "--help, -h\t\t" << "Prints this screen." << std::endl
                  << "-dt <value>\t\t" << "Sets delta time to <value>. If -dt is not specified default value is used."
                  << std::endl
                  << "-et <value>\t\t" << "Set end time to <value>. If -et is not specified default value is used."
                  << std::endl
                  << "-o <name>\t\t" << "Set base name of output files. DO NOT USE A PATH! Default is 'result'."
                  << std::endl
                  << "-of <path>\t\t" << "Set path to output folder. Default is ./output"
                  << std::endl
                  << "-llv <int>\t\t" << "Set log level. 0: trace 1: debug 2: info 3: warn 4: err 5: critical 6: off"
                  << std::endl;

    }

    ArgsParser::ArgsParser(int argc, char *argsv[]) {
        for (int index{1}; index < argc; index++) {
            if (argsv[index][0] == '-' &&
                std::find(args.begin(), args.end(), std::string{argsv[index]}) != args.end())
                exitFormatError("Option: " + std::string{argsv[index]} + " already exists.");
            args.emplace_back(std::string{argsv[index]});
        }

        if (optionExists("-h") || optionExists("--help")) {
            printHelp();
            exit(EXIT_SUCCESS);
        }
    }

    bool ArgsParser::optionExists(const std::string &op) {
        return std::any_of(args.begin(), args.end(), [&](const std::string &s) { return s == op; });
    }

    bool ArgsParser::optionArgExists(const std::string &op) {
        if (!optionExists(op)) return false;
        auto it = std::find(args.begin(), args.end(), op);
        // end of vector
        if (it == args.end() - 1) return false;
        // not end -> check for arg
        if ((it + 1)->operator[](0) == '-') return false;
        return true;
    }

    std::string ArgsParser::getOptionArg(const std::string &op) {
        auto it = std::find(args.begin(), args.end(), op);
        return *(it + 1);
    }

    void ArgsParser::getInputPaths(std::vector<std::string> &buffer) {
        for (unsigned int index{0}; index < args.size(); index++) {
            if (args[index].c_str()[0] == '-') {
                if (optionArgExists(args[index])) index++;
                continue;
            }

            buffer.emplace_back(args[index]);
        }
    }
} // cli