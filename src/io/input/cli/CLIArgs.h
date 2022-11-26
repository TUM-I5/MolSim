//
// Created by alex on 26.11.2022.
//

#pragma once

#include <utility>
#include <variant>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

namespace io::input {

    /**
     * Entry for cli_arg_map.
     * Will be used to extract all arguments from cli input.
     * */
    template<typename V>
    struct ArgEntry {
        /**Short name of cli arg: eg. call with -<arg>*/
        std::string_view shortName;
        /**Long name of cli arg: eg. call with --<arg>*/
        std::string_view longName;
        /**Will a parameter be expected?*/
        bool expectParam;
        /**Extracts the parameter from string to wished type. Result will be stored in value*/
        std::function<V(std::string &)> extractor;
        /**Will be called after the extractor or with default value. Value will be passed to this function*/
        std::function<void(const V&)> handler;
        /**Default value if arg is not specified*/
        V defaultValue;
        /**Description text. Is used to print help page*/
        std::string_view description;
        /**Param text. Is used to print help page*/
        std::string_view paramText;
        /**Signalizes if the arg was set or not*/
        bool isSet;
        /**Storage for param extraction*/
        V value;


        ArgEntry(const std::string &sn,
                 const std::string &ln,
                 const std::string &desc,
                 const std::string &pTxt,
                 bool ep,
                 V dVal,
                 std::function<V(std::string &)> ext = [](std::string&)->V{},
                 std::function<void(const V&)> hand = [](const V&){}
        ) : shortName(sn),
            longName(ln),
            expectParam(ep),
            extractor(std::move(ext)),
            handler(std::move(hand)),
            defaultValue(dVal),
            description(desc),
            paramText(pTxt) {
            isSet = false;
        }
    };

    using ArgEntry_t = std::variant<ArgEntry<int>, ArgEntry<double>, ArgEntry<std::string>>;
    using cli_arg_map_t = std::unordered_map<std::string, ArgEntry_t>;

    /**
     * Registry for all cli arguments.
     * */
    extern cli_arg_map_t cli_arg_map;

} // io::input

