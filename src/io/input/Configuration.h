//
// Created by alex on 30.11.22.
//

#pragma once

#include <string>
#include <variant>
#include <iostream>
#include "sim/physics/bounds/types.h"
#include "sim/physics/force/types.h"
#include "sim/physics/velocity/types.h"
#include "sim/physics/position/types.h"
#include "arg_names.h"
#include "io/input/cli/CLIArgs.h"

namespace io::input {

    using force_t = sim::physics::force::type;
    using pos_t = sim::physics::position::type;
    using vel_t = sim::physics::velocity::type;
    using bound_t = sim::physics::bounds::type;

    struct ERROR_T{};

    /**
     * Defines a map from enum name to new data type
     * */
    template<io::input::names N>
    struct map{
        template<bool b, typename T, typename F>
        struct cond {
            using type = T;
        };

        template<typename T, typename F>
        struct cond<false, T, F> {
            using type = F;
        };

        /**
         * Simplifies the condition
         * */
        template<io::input::names M, typename T, typename F>
        struct entry{
            using type = typename cond<N==M, T, F>::type;
        };


        /**
         * type of lookup
         * */
        using type = typename entry<outputFilePath, std::string,
                     typename entry<outputFileName, std::string,
                     typename entry<startTime, double,
                     typename entry<endTime, double,
                     typename entry<delta_t, double,
                     typename entry<forceCalculation, force_t,
                     typename entry<positionCalculation, pos_t,
                     typename entry<velocityCalculation, vel_t,
                     typename entry<sigma, double,
                     typename entry<epsilon, double,
                     typename entry<brown, double,
                     typename entry<linkedCell, bool,
                     typename entry<rCutoff, double,
                     typename entry<boundingBox_X0, double,
                     typename entry<boundingBox_X1, double,
                     typename entry<boundingBox_X2, double,
                     typename entry<boundCondFront, bound_t,
                     typename entry<boundCondRear, bound_t,
                     typename entry<boundCondLeft, bound_t,
                     typename entry<boundCondRight, bound_t,
                     typename entry<boundCondTop, bound_t,
                     typename entry<boundCondBottom, bound_t,
                     typename entry<dimensions, int,
                     typename entry<logLevel, int,
                     typename entry<benchmark, bool,
                     typename entry<benchmarkType, std::string,
                     typename entry<benchMaxBodySize, int,
                     typename entry<benchIterationCount, int,
                ERROR_T>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type>::type;
    };

    /**
     * Manages all input parameters. Will first acquire params from CLI and then File.
     * */
    class Configuration {

    private:
        //value lock
        std::unordered_map<names, bool> valueLock;

        //data storage
        using config_entry_t = std::variant<std::string, double, int, bool, force_t, pos_t, vel_t, bound_t, ERROR_T>;
        std::unordered_map<names, config_entry_t> dataStorage;

    public:

        template<names N, typename R = typename map<N>::type>
        R get() {
            return std::get<R>(dataStorage[N]);
        }

        /**
         * Loads in all args from the cli registry.
         * CLIArgsParser::parseArgs() has to be called before.
         * */
        void loadCLIArgs() {
            dataStorage[outputFilePath] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-of")).value;
            dataStorage[outputFileName] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-o")).value;
            dataStorage[startTime] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-st")).value;
            dataStorage[endTime] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-et")).value;
            dataStorage[delta_t] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-dt")).value;
            dataStorage[forceCalculation] = sim::physics::force::stot(std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-f")).value);
            dataStorage[positionCalculation] = sim::physics::position::stot(std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-x")).value);
            dataStorage[velocityCalculation] = sim::physics::velocity::stot(std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-v")).value);
            dataStorage[sigma] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-sig")).value;
            dataStorage[epsilon] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-eps")).value;
            dataStorage[brown] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-brown")).value;
            dataStorage[linkedCell] = std::get<io::input::ArgEntry<int>>(cli_arg_map.at("-lc")).value != 0;
            dataStorage[rCutoff] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-rc")).value;
            dataStorage[boundingBox_X0] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-bbox0")).value;
            dataStorage[boundingBox_X1] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-bbox1")).value;
            dataStorage[boundingBox_X2] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-bbox2")).value;
            dataStorage[boundCondFront] = sim::physics::bounds::stot(std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndFront")).value);
            dataStorage[boundCondRear] = sim::physics::bounds::stot(std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndRear")).value);
            dataStorage[boundCondLeft] = sim::physics::bounds::stot(std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndLeft")).value);
            dataStorage[boundCondRight] = sim::physics::bounds::stot(std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndRight")).value);
            dataStorage[boundCondTop] = sim::physics::bounds::stot(std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndTop")).value);
            dataStorage[boundCondBottom] = sim::physics::bounds::stot(std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndBottom")).value);
            dataStorage[dimensions] = std::get<io::input::ArgEntry<int>>(cli_arg_map.at("-dims")).value;
            dataStorage[logLevel] = std::get<io::input::ArgEntry<int>>(cli_arg_map.at("-llv")).value;
            dataStorage[benchmark] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bench")).isSet;
            dataStorage[benchmarkType] = std::get<io::input::ArgEntry<std::string>>(io::input::cli_arg_map.at("-bench")).value;
            dataStorage[benchMaxBodySize] = std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-bMax")).value;
            dataStorage[benchIterationCount] = std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-i")).value;

            valueLock[outputFilePath] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-of")).isSet;
            valueLock[outputFileName] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-o")).isSet;
            valueLock[startTime] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-st")).isSet;
            valueLock[endTime] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-et")).isSet;
            valueLock[delta_t] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-dt")).isSet;
            valueLock[forceCalculation] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-f")).isSet;
            valueLock[positionCalculation] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-x")).isSet;
            valueLock[velocityCalculation] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-v")).isSet;
            valueLock[sigma] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-sig")).isSet;
            valueLock[epsilon] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-eps")).isSet;
            valueLock[brown] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-brown")).isSet;
            valueLock[linkedCell] = std::get<io::input::ArgEntry<int>>(cli_arg_map.at("-lc")).isSet;
            valueLock[rCutoff] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-rc")).isSet;
            valueLock[boundingBox_X0] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-bbox0")).isSet;
            valueLock[boundingBox_X1] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-bbox1")).isSet;
            valueLock[boundingBox_X2] = std::get<io::input::ArgEntry<double>>(cli_arg_map.at("-bbox2")).isSet;
            valueLock[boundCondFront] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndFront")).isSet;
            valueLock[boundCondRear] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndRear")).isSet;
            valueLock[boundCondLeft] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndLeft")).isSet;
            valueLock[boundCondRight] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndRight")).isSet;
            valueLock[boundCondTop] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndTop")).isSet;
            valueLock[boundCondBottom] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bndBottom")).isSet;
            valueLock[dimensions] = std::get<io::input::ArgEntry<int>>(cli_arg_map.at("-dims")).isSet;
            valueLock[logLevel] = std::get<io::input::ArgEntry<int>>(cli_arg_map.at("-llv")).isSet;
            valueLock[benchmark] = std::get<io::input::ArgEntry<std::string>>(cli_arg_map.at("-bench")).isSet;
            valueLock[benchmarkType] = std::get<io::input::ArgEntry<std::string>>(io::input::cli_arg_map.at("-bench")).isSet;
            valueLock[benchMaxBodySize] = std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-bMax")).isSet;
            valueLock[benchIterationCount] = std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-i")).isSet;
        }
        
        /**
         * Loads in all args from the provided arg map. This should be from the IOWrapper.
         * */
        void loadIOWArgs(const std::unordered_map<names, std::string>& argMap) {
            if(!valueLock[outputFilePath] && argMap.contains(outputFilePath)) dataStorage[outputFilePath] = argMap.at(outputFilePath);
            if(!valueLock[outputFileName] && argMap.contains(outputFileName)) dataStorage[outputFileName] = argMap.at(outputFileName);
            if(!valueLock[startTime] && argMap.contains(startTime)) dataStorage[startTime] = std::stod(argMap.at(startTime));
            if(!valueLock[endTime] && argMap.contains(endTime)) dataStorage[endTime] = std::stod(argMap.at(endTime));
            if(!valueLock[delta_t] && argMap.contains(delta_t)) dataStorage[delta_t] = std::stod(argMap.at(delta_t));
            if(!valueLock[forceCalculation] && argMap.contains(forceCalculation)) dataStorage[forceCalculation] = sim::physics::force::stot(argMap.at(forceCalculation));
            if(!valueLock[positionCalculation] && argMap.contains(positionCalculation)) dataStorage[positionCalculation] = sim::physics::position::stot(argMap.at(positionCalculation));
            if(!valueLock[velocityCalculation] && argMap.contains(velocityCalculation)) dataStorage[velocityCalculation] = sim::physics::velocity::stot(argMap.at(velocityCalculation));
            if(!valueLock[sigma] && argMap.contains(sigma)) dataStorage[sigma] = std::stod(argMap.at(sigma));
            if(!valueLock[epsilon] && argMap.contains(epsilon)) dataStorage[epsilon] = std::stod(argMap.at(epsilon));
            if(!valueLock[brown] && argMap.contains(brown)) dataStorage[brown] = std::stod(argMap.at(brown));
            if(!valueLock[linkedCell] && argMap.contains(linkedCell)) dataStorage[linkedCell] = std::stoi(argMap.at(linkedCell)) != 0;
            if(!valueLock[rCutoff] && argMap.contains(rCutoff)) dataStorage[rCutoff] = std::stod(argMap.at(rCutoff));
            if(!valueLock[boundingBox_X0] && argMap.contains(boundingBox_X0)) dataStorage[boundingBox_X0] = std::stod(argMap.at(boundingBox_X0));
            if(!valueLock[boundingBox_X1] && argMap.contains(boundingBox_X1)) dataStorage[boundingBox_X1] = std::stod(argMap.at(boundingBox_X1));
            if(!valueLock[boundingBox_X2] && argMap.contains(boundingBox_X2)) dataStorage[boundingBox_X2] = std::stod(argMap.at(boundingBox_X2));
            if(!valueLock[boundCondFront] && argMap.contains(boundCondFront)) dataStorage[boundCondFront] = sim::physics::bounds::stot(argMap.at(boundCondFront));
            if(!valueLock[boundCondRear] && argMap.contains(boundCondRear)) dataStorage[boundCondRear] = sim::physics::bounds::stot(argMap.at(boundCondRear));
            if(!valueLock[boundCondLeft] && argMap.contains(boundCondLeft)) dataStorage[boundCondLeft] = sim::physics::bounds::stot(argMap.at(boundCondLeft));
            if(!valueLock[boundCondRight] && argMap.contains(boundCondRight)) dataStorage[boundCondRight] = sim::physics::bounds::stot(argMap.at(boundCondRight));
            if(!valueLock[boundCondTop] && argMap.contains(boundCondTop)) dataStorage[boundCondTop] = sim::physics::bounds::stot(argMap.at(boundCondTop));
            if(!valueLock[boundCondBottom] && argMap.contains(boundCondBottom)) dataStorage[boundCondBottom] = sim::physics::bounds::stot(argMap.at(boundCondBottom));
            if(!valueLock[dimensions] && argMap.contains(dimensions)) dataStorage[dimensions] = std::stoi(argMap.at(dimensions));
            if(!valueLock[logLevel] && argMap.contains(logLevel)) dataStorage[logLevel] = std::stoi(argMap.at(logLevel));
            if(!valueLock[benchmark] && argMap.contains(benchmark)) dataStorage[benchmark] = std::stoi(argMap.at(benchmark)) != 0;
            if(!valueLock[benchmarkType] && argMap.contains(benchmarkType)) dataStorage[benchmarkType] = argMap.at(benchmarkType);
            if(!valueLock[benchMaxBodySize] && argMap.contains(benchMaxBodySize)) dataStorage[benchMaxBodySize] = std::stoi(argMap.at(benchMaxBodySize));
            if(!valueLock[benchIterationCount] && argMap.contains(benchIterationCount)) dataStorage[benchIterationCount] = std::stoi(argMap.at(benchIterationCount));
        }
    };

} // input
