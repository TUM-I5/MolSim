#include <gtest/gtest.h>
#include "utils/ForceCalculations.h"



/**
 * @brief Test the forceCalculation with hand-calculated values
 * 
*/
TEST(forceCalculation,advanced_test_force_calculations){
    auto force_func = forceLennJonesPotentialFunction(1.0,5.0);

    Particle p1({2.0,3,4},{0,0,0},1);
    Particle p2({5.0,6,7},{0,0,0},1);

    auto res = force_func(p1,p2);

    std::cout << "Elements of the array 'res': ";
    for (const auto& elem : res) {
        std::cout << elem << " ";
    }

    std::cout << "===============================================================" << std::endl;

    ASSERT_NEAR(res[0],0.00067733,1e-6);
    ASSERT_NEAR(res[1],0.00067733,1e-6);
    ASSERT_NEAR(res[2],0.00067733,1e-6);
}

/**
 * @brief Test the forceCalculation with hand-calculated values
 * 
*/
TEST(forceCalculation,advanced_test_force_calculations1){
    auto force_func = forceLennJonesPotentialFunction(1.0,5.0);

    Particle p1({5.0,3.0,4.5},{0,0,0},1);
    Particle p2({5.0,6.3,7.4},{0,0,0},1);


    auto res = force_func(p1,p2);

    std::cout << "Elements of the array 'res': ";
    for (const auto& elem : res) {
        std::cout << elem << " ";
    }
    std::cout << "===============================================================" << std::endl;

    ASSERT_NEAR(res[0],-0.0,1e-6);
    ASSERT_NEAR(res[1],0.00285328,1e-6);
    ASSERT_NEAR(res[2],0.00250743,1e-6);
}


/**
 * @brief Test the forceCalculation with hand-calculated values
 * 
*/
TEST(forceCalculation,advanced_test_force_calculations2){
    auto force_func = forceLennJonesPotentialFunction(1.0,5.0);



    Particle p1({5.23,2.121,4.5},{0,0,0},1);
    Particle p2({1.1213,6.30,2.23},{0,0,0},1);


    auto res = force_func(p1,p2);

    std::cout << "Elements of the array 'res': ";
    for (const auto& elem : res) {
        std::cout << elem << " ";
    }
    std::cout << "===============================================================" << std::endl;


    ASSERT_NEAR(res[0],-0.00020256 ,1e-6);
    ASSERT_NEAR(res[1],0.00020603,1e-6);
    ASSERT_NEAR(res[2],-0.00011191,1e-6);
}