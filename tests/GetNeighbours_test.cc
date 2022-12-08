#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/utils/PContainer.h"

#include <vector>
#include <array>

using namespace PContainer;

// checks whether index conversion from 1D to 3D works as expected
TEST(convert1DTo3Dim, Basic)
{
    std::array<int, 3> numCells;
    std::array<int, 3> result;

    // FIRST EXAMPLE
    /*
     * 6 7 8 . 15 16 17 . 24 25 26
     * 3 4 5 . 12 13 14 . 21 22 23
     * 0 1 2 .  9 10 11 . 18 19 20
     */

    numCells[0] = 3;
    numCells[1] = 3;
    numCells[2] = 3;

    result = convert1DTo3D(22, numCells);

    EXPECT_TRUE(result.at(0) == 1 && result.at(1) == 1 && result.at(2) == 2);

    result = convert1DTo3D(11, numCells);

    EXPECT_TRUE(result.at(0) == 2 && result.at(1) == 0 && result.at(2) == 1);

    // SECOND EXAMPLE
    /*
     * 8  9 10 11 . 20 21 22 23
     * 4  5  6  7 . 16 17 18 19
     * 0  1  2  3 . 12 13 14 15
     */

    numCells[0] = 4;
    numCells[1] = 3;
    numCells[2] = 2;

    result = convert1DTo3D(22, numCells);

    EXPECT_TRUE(result.at(0) == 2 && result.at(1) == 2 && result.at(2) == 1);

    result = convert1DTo3D(6, numCells);

    EXPECT_TRUE(result.at(0) == 2 && result.at(1) == 1 && result.at(2) == 0);
}

// checks whether index conversion from 3D to 1D works as expected
TEST(convert3DTo1D, Basic)
{
    std::array<int, 3> numCells;
    std::array<int, 3> index;
    int result;

    // FIRST EXAMPLE
    /*
     * 6 7 8 . 15 16 17 . 24 25 26
     * 3 4 5 . 12 13 14 . 21 22 23
     * 0 1 2 .  9 10 11 . 18 19 20
     */

    numCells[0] = 3;
    numCells[1] = 3;
    numCells[2] = 3;

    // 17
    index[0] = 2;
    index[1] = 2;
    index[2] = 1;

    EXPECT_TRUE(convert3DTo1D(index, numCells) == 17);

    // 20
    index[0] = 2;
    index[1] = 0;
    index[2] = 2;

    EXPECT_TRUE(convert3DTo1D(index, numCells) == 20);

    // SECOND EXAMPLE
    /*
     * 8  9 10 11 . 20 21 22 23
     * 4  5  6  7 . 16 17 18 19
     * 0  1  2  3 . 12 13 14 15
     */

    numCells[0] = 4;
    numCells[1] = 3;
    numCells[2] = 2;

    // 11
    index[0] = 3;
    index[1] = 2;
    index[2] = 0;

    EXPECT_TRUE(convert3DTo1D(index, numCells) == 11);

    // 19
    index[0] = 3;
    index[1] = 1;
    index[2] = 1;

    EXPECT_TRUE(convert3DTo1D(index, numCells) == 19);
}

// checks whether getNeighboursNewton returns the right value for a given cell
// 2 Dimensional => z = 1
TEST(getNeighboursNewton, TwoDimTest)
{
    std::array<int, 3> numCells;
    std::vector<int> result;
    /*
     * 6 7 8
     * 3 4 5
     * 0 1 2
     */

    numCells[0] = 3;
    numCells[1] = 3;
    numCells[2] = 1;

    // 0
    result = getNeighboursNewton(0, numCells);

    EXPECT_TRUE(result.size() == 3);
    EXPECT_TRUE(result.at(0) == 1 && result.at(1) == 3 && result.at(2) == 4);

    // 4
    result = getNeighboursNewton(4, numCells);
    EXPECT_TRUE(result.size() == 4);
    EXPECT_TRUE(result.at(0) == 5 && result.at(1) == 6 && result.at(2) == 7 && result.at(3) == 8);

    // 8
    result = getNeighboursNewton(8, numCells);
    EXPECT_TRUE(result.size() == 0);
}

// checks whether getNeighboursNewton returns the right value for a given cell
// 2 Dimensional => z >= 1
TEST(getNeighboursNewton, ThreeDimTest)
{
    std::array<int, 3> numCells;
    std::vector<int> result;
    /*
     * 9 10 11 . 21 22 23
     * 6  7  8 . 18 19 20
     * 3  4  5 . 15 16 17
     * 0  1  2 . 12 13 14
     */

    numCells[0] = 3;
    numCells[1] = 4;
    numCells[2] = 2;

    // 0
    result = getNeighboursNewton(0, numCells);

    EXPECT_TRUE(result.size() == 7);
    EXPECT_TRUE(result.at(0) == 1 && result.at(1) == 3 && result.at(2) == 4 && result.at(3) == 12 && result.at(4) == 13 && result.at(5) == 15 && result.at(6) == 16);

    // 4
    result = getNeighboursNewton(4, numCells);

    EXPECT_TRUE(result.size() == 13);
    EXPECT_TRUE(result.at(0) == 5 && result.at(1) == 6 && result.at(2) == 7 && result.at(3) == 8 && result.at(4) == 16 && result.at(5) == 12 && result.at(6) == 13 && result.at(7) == 14 && result.at(8) == 15 && result.at(9) == 17 && result.at(10) == 18 && result.at(11) == 19 && result.at(12) == 20);

    // 7
    result = getNeighboursNewton(7, numCells);

    EXPECT_TRUE(result.size() == 13);
    EXPECT_TRUE(result.at(0) == 8 && result.at(1) == 9 && result.at(2) == 10 && result.at(3) == 11 && result.at(4) == 19 && result.at(5) == 15 && result.at(6) == 16 && result.at(7) == 17 && result.at(8) == 18 && result.at(9) == 20 && result.at(10) == 21 && result.at(11) == 22 && result.at(12) == 23);

    // 11
    result = getNeighboursNewton(11, numCells);

    EXPECT_TRUE(result.size() == 4);
    EXPECT_TRUE(result.at(0) == 23 && result.at(1) == 19 && result.at(2) == 20 && result.at(3) == 22);

    // 20
    result = getNeighboursNewton(20, numCells);

    EXPECT_TRUE(result.size() == 2);
    EXPECT_TRUE(result.at(0) == 22 && result.at(1) == 23);
}