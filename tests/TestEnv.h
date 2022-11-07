//
// Created by alex on 07.11.2022.
//
#pragma once

#include <gtest/gtest.h>
#include "io/Logging.h"
#include "Simulation.h"

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
        loggers::init();
        sim::start_time = 0;
        sim::end_time = 1000;
        sim::delta_t = 0.014;
        sim::epsilon = 1.0;
        sim::sigma = 1;
    }
    void TearDown() override {}
};

/**
 * Here TestEnv will be registered to googletest as the global testing environment.
 * This is done purely statically.
 * */
extern testing::Environment* const testEnv;


