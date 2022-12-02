//
// Created by alex on 29.10.2022.
//

#include "CLIArgsParser.h"
#include "CLIArgs.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

namespace io::input {
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
                  << "--help, -h\t\t" << "Prints this screen." << std::endl;
        // print other args in arg registry
        for (const auto& [_, entry] : io::input::cli_arg_map) {
            std::visit([](const auto& e){
                std::cout << e.shortName << "," << e.longName;
                if (e.expectParam) std::cout << " " << e.paramText << " ";
                std::cout << std::endl;
                std::cout << "\t\t" << e.description << std::endl;
            }, entry);

        }
    }

    CLIArgsParser::CLIArgsParser(int argc, char *argsv[]) {
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

    bool CLIArgsParser::optionExists(const std::string_view &op) {
        return std::any_of(args.begin(), args.end(), [&](const std::string &s) { return s == op; });
    }

    bool CLIArgsParser::optionArgExists(const std::string_view &op) {
        if (!optionExists(op)) return false;
        auto it = std::find(args.begin(), args.end(), op);
        // end of vector
        if (it == args.end() - 1) return false;
        // not end -> check for arg
        if ((it + 1)->operator[](0) == '-') return false;
        return true;
    }

    std::string CLIArgsParser::getOptionArg(const std::string_view &op) {
        auto it = std::find(args.begin(), args.end(), op);
        return *(it + 1);
    }

    void CLIArgsParser::getInputPaths(std::vector<std::string> &buffer) {
        for (unsigned int index{0}; index < args.size(); index++) {
            if (args[index].c_str()[0] == '-') {
                if (optionArgExists(args[index])) index++;
                continue;
            }

            buffer.emplace_back(args[index]);
        }
    }

    void CLIArgsParser::parseArgs(){
        for (auto& [sn, entry] : io::input::cli_arg_map) {
            std::visit([this](auto& e){
                //check if arg is set
                if (optionExists(e.shortName) || optionExists(e.longName)) e.isSet = true;

                //call extractor only if param is required
                if (e.expectParam) {

                    //check short name
                    if (optionArgExists(e.shortName)) {
                        std::string arg = getOptionArg(e.shortName);
                        e.value = e.extractor(arg);
                    }
                    //check long name
                    else if (optionArgExists(e.longName)) {
                        std::string arg = getOptionArg(e.longName);
                        e.value = e.extractor(arg);
                    }
                    else e.value = e.defaultValue;
                }

                //call handler
                e.handler(e.value);
            }, entry);
        }
    }
} // io::input
