//
// Created by alex on 25.11.2022.
//

#pragma once


namespace io::input {
    /**
    * Defines names in the map for arguments in the input loader
    * */
    enum names {
        outputFilePath,
        outputFileName,
        startTime,
        endTime,
        timeStepSize,
        forceCalculation,
        sigma,
        epsilon,
        brown,
        linkedCell,
        boundingBox_X0,
        boundingBox_X1,
        boundingBox_X2,
        boundCondFront,
        boundCondRear,
        boundCondLeft,
        boundCondRight,
        boundCondTop,
        boundCondBottom,
        dimensions,
        logLevel,
        benchmark,
        benchmarkType,
        benchMaxBodySize,
        benchIterationCount,
        names_count [[maybe_unused]]
    };
} // io::input