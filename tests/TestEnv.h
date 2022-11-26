//
// Created by alex on 07.11.2022.
//
#pragma once

#include <gtest/gtest.h>
#include "io/output/Logging.h"
#include "simulation/Simulation.h"

/**
 * Define an Environment for all tests.
 * All loggers will be initialized here.
 * */
class TestEnv : public ::testing::Environment {
public:
    ~TestEnv() override {}
    /**
     * Will be called by googletest.
     * Initialize all things that have to be initialized here!
     * If other values are desired in a test, then override them there.
     * */
    void SetUp() override {
        loggers::init(loggers::level::trace);
        sim::Simulation<>();
    }
    void TearDown() override {}
};

/**
 * Here TestEnv will be registered to googletest as the global testing environment.
 * This is done purely statically.
 * */
extern testing::Environment* const testEnv;


