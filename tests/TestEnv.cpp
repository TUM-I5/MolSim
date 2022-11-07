//
// Created by alex on 07.11.2022.
//

#include "TestEnv.h"

testing::Environment* const testEnv = testing::AddGlobalTestEnvironment(new TestEnv);