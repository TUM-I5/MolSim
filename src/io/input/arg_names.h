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
        delta_t,
        forceCalculation,
        positionCalculation,
        velocityCalculation,
        sigma,
        epsilon,
        brown,
        linkedCell,
        rCutoff,
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
        thermoEnable,
        thermoTInit,
        thermoNTerm,
        thermoTTarget,
        thermoDelta_t,
        checkpointingEnable,
        simLastIteration,
        gGrav,
        names_count [[maybe_unused]]
    };
} // io::input