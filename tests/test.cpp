#include <gtest/gtest.h>
#include "MolSim.cpp"



TEST(testMolSim,test_testfunc){
    EXPECT_EQ(testfunc(),10);
}



TEST(testMolSim,test_testfunc2){
    EXPECT_EQ(testfunc(),8);
}