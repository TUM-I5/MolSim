#include <gtest/gtest.h>
#include "particleModel/storage/CellContainer.h"

void skipStartVariants(int amount, CellContainer &cellContainer, std::array<dim_t,3> &start, std::array<dim_t,3> &pattern) {
    for(int i = 0; i < amount; i++) {
        cellContainer.setNextPath(start, pattern);
    }
}

/**
 * @brief
*/
TEST(cellcontainer, test_setNextCell) {
    CellContainer cellContainer0{4.0,2.0,2.0,3.0,3.0};
    CellContainer cellContainer1{4.0,2.0,2.0,3.0,3.0};
    CellContainer cellContainer2{9.0,9.0,9.0,3.0,3.0};
    std::array<dim_t,3> position{0,0,0};
    std::array<double,3> x111{0,0,0};
    std::array<double,3> x123{0,5,8};
    std::array<double,3> x333{8,8,8};
    std::array<double,3> v{0,0,0};
    double m = 1;

    //test correct iteration with empty CellContainer of domain dimension 2,1,1
    cellContainer0.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);

    cellContainer0.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);


    //test correct iteration with CellContainer of domain dimension 2,1,1 and non-empty cells
    cellContainer1.addParticle(x111, v, m);
    cellContainer1.createPointers();

    cellContainer1.setNextCell(position);
    ASSERT_EQ(position[0], 1);
    ASSERT_EQ(position[1], 1);
    ASSERT_EQ(position[2], 1);

    cellContainer1.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);

    cellContainer1.setNextCell(position);
    ASSERT_EQ(position[0], 1);
    ASSERT_EQ(position[1], 1);
    ASSERT_EQ(position[2], 1);

    cellContainer1.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);


    //test correct iteration with CellContainer of domain dimension 3,3,3
    cellContainer2.addParticle(x111, v, m);
    cellContainer2.addParticle(x123, v, m);
    cellContainer2.addParticle(x333, v, m);
    cellContainer2.createPointers();

    cellContainer2.setNextCell(position);
    ASSERT_EQ(position[0], 1);
    ASSERT_EQ(position[1], 1);
    ASSERT_EQ(position[2], 1);

    cellContainer2.setNextCell(position);
    ASSERT_EQ(position[0], 1);
    ASSERT_EQ(position[1], 2);
    ASSERT_EQ(position[2], 3);

    cellContainer2.setNextCell(position);
    ASSERT_EQ(position[0], 3);
    ASSERT_EQ(position[1], 3);
    ASSERT_EQ(position[2], 3);

    cellContainer2.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);
}

/**
 * @brief
*/
TEST(cellcontainer, test_setNextPath1) {
    CellContainer cellContainer3{4.0, 2.0, 2.0, 3.0, 3.0};
    std::array<dim_t,3> start{0,0,0};
    std::array<dim_t,3> pattern{0,0,0};

    //test correct paths for domain dimension 2,1,1
    cellContainer3.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], 0);

    cellContainer3.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], -1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer3.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], -1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer3.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 0);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer3.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 0);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer3.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer3.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer3.setNextPath(start, pattern);
    ASSERT_EQ(start[0], dim_t_res);

    cellContainer3.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], 0);

    skipStartVariants(7, cellContainer3, start, pattern);
}

/**
 * @brief
*/
TEST(cellcontainer, test_setNextPath2) {
    CellContainer cellContainer4{4.0, 4.0, 2.0, 3.0, 3.0};
    std::array<dim_t,3> start{0,0,0};
    std::array<dim_t,3> pattern{0,0,0};

    //test correct paths for domain dimension 2,2,1
    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], -1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], -1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], -1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 0);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 0);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], dim_t_res);

    cellContainer4.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], 0);

    skipStartVariants(10, cellContainer4, start, pattern);
}

/**
 * @brief
*/
TEST(cellcontainer, test_setNextPath3) {
    CellContainer cellContainer5{4.0, 4.0, 4.0, 3.0, 3.0};
    std::array<dim_t,3> start{0,0,0};
    std::array<dim_t,3> pattern{0,0,0};

    //test correct paths for domain dimension 2,2,2
    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 0);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], -1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], 0);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], -1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], -1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 2);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 2);
    ASSERT_EQ(start[2], 2);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(pattern[0], 0);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], -1);

    skipStartVariants(6, cellContainer5, start, pattern);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], -1);

    skipStartVariants(7, cellContainer5, start, pattern);
    ASSERT_EQ(pattern[0], -1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    skipStartVariants(6, cellContainer5, start, pattern);
    ASSERT_EQ(pattern[0], 0);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    skipStartVariants(4, cellContainer5, start, pattern);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 0);

    skipStartVariants(6, cellContainer5, start, pattern);
    ASSERT_EQ(pattern[0], -1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 1);

    skipStartVariants(7, cellContainer5, start, pattern);
    ASSERT_EQ(pattern[0], 0);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 1);

    skipStartVariants(6, cellContainer5, start, pattern);
    ASSERT_EQ(pattern[0], 1);
    ASSERT_EQ(pattern[1], 1);
    ASSERT_EQ(pattern[2], 1);

    skipStartVariants(7, cellContainer5, start, pattern);
    ASSERT_EQ(start[0], dim_t_res);

    cellContainer5.setNextPath(start, pattern);
    ASSERT_EQ(start[0], 1);
    ASSERT_EQ(start[1], 1);
    ASSERT_EQ(start[2], 1);
    ASSERT_EQ(pattern[0], 0);
    ASSERT_EQ(pattern[1], 0);
    ASSERT_EQ(pattern[2], 1);

    skipStartVariants(76, cellContainer5, start, pattern);
}

/**
 * @brief
*/
TEST(cellcontainer, test_setNextPath4) {
    CellContainer cellContainer6{4.0, 4.0, 2.0, 3.0, 1.5};
    std::array<dim_t,3> start{0,0,0};
    std::array<dim_t,3> pattern{0,0,0};
    int skip, x, y, z;
    //all patterns for comparing_depth 2
    std::vector<std::array<dim_t,3>> check{
        {0,0,1}, {0,0,2}, {1,0,-2}, {2,0,-2}, {1,0,-1}, {2,0,-1}, {1,0,0}, {2,0,0},
        {1,0,1}, {2,0,1}, {1,0,2}, {2,0,2}, {-2,1,-2}, {-1,1,-2}, {0,1,-2}, {1,1,-2},
        {2,1,-2}, {-2,2,-2}, {-1,2,-2}, {0,2,-2}, {1,2,-2}, {2,2,-2}, {-2,1,-1},
        {-1,1,-1}, {0,1,-1}, {1,1,-1}, {2,1,-1}, {-2,2,-1}, {-1,2,-1}, {0,2,-1},
        {1,2,-1}, {2,2,-1}, {-2,1,0}, {-1,1,0}, {0,1,0}, {1,1,0}, {2,1,0}, {-2,2,0},
        {-1,2,0}, {0,2,0}, {1,2,0}, {2,2,0}, {-2,1,1}, {-1,1,1}, {0,1,1}, {1,1,1},
        {2,1,1}, {-2,2,1}, {-1,2,1}, {0,2,1}, {1,2,1}, {2,2,1}, {-2,1,2}, {-1,1,2},
        {0,1,2}, {1,1,2}, {2,1,2}, {-2,2,2}, {-1,2,2}, {0,2,2}, {1,2,2}, {2,2,2}};

    //test correct paths for domain dimension 3,3,2 and comparing_depth 2
    cellContainer6.setNextPath(start, pattern);
    ASSERT_EQ(pattern, check[0]);
    skipStartVariants(9, cellContainer6, start, pattern);

    for(int i = 1; i < check.size(); i++) {
        ASSERT_EQ(pattern, check[i]);

        x = abs(check[i][0]);
        y = abs(check[i][1]);
        z = abs(check[i][2]);

        //determine the amount of start positions to skip based on the pattern
        if(z == 0) {
            if(x != 0 && y != 0) {
                skip = 6 * (x + y) - pow(2, x + y - 1);

            } else {
                skip = 6 * (x + y);
            }
        } else if(z == 1 && (x == 0 || y == 0)) {
            skip = 3 * x + 3 * y + 9;

        } else if(z == 2) {
            skip = 18;

        } else { //z == 1 && x != 0 && y != 0
            if(x + y == 3) {
                skip = 16;
            } else if(x + y == 4) {
                skip = 17;
            } else {
                skip = 14;
            }
        }

        skipStartVariants(skip, cellContainer6, start, pattern);
    }

    ASSERT_EQ(start[0], dim_t_res);
}

/**
 * @brief
*/
TEST(cellcontainer, test_setNextPath5) {
    CellContainer cellContainer7{4.0, 4.0, 4.0, 3.0, 1.0};
    std::array<dim_t,3> start{0,0,0};
    std::array<dim_t,3> pattern{0,0,0};

    //domain dimensions 4,4,4
    //patterns that have to come up in the process
    std::vector<std::array<dim_t,3>> checklist{{3,3,3}, {2,2,2}, {-3,3,-3},
                                               {0,0,3}, {1,1,1},{2,1,1}, {-2,2,1},
                                               {-1,2,1}, {0,2,1}, {1,2,1}, {2,2,1},
                                               {-2,1,2}, {-1,1,2},{0,0,1}, {0,0,2},
                                               {1,0,-2}, {2,0,-2}, {1,0,-1}, {2,0,-1},
                                               {1,0,0}, {2,0,0}, {1,0,1}, {2,0,1},
                                               {1,0,2}, {2,0,2}, {-2,1,-2}, {-1,1,-2},
                                               {0,1,-2}, {1,1,-3},{2,1,-2}, {-3,2,-2},
                                               {-1,2,-2}, {0,2,-2}, {1,3,-2}, {2,2,-2},
                                               {-2,1,-1},{-1,1,-3}, {0,1,-1}, {1,1,-1},
                                               {3,1,-1}, {-2,2,-1}, {-1,2,-1}, {0,2,-1}};

    while(start[0] != dim_t_res) {
        ASSERT_LE(0,start[0]);
        ASSERT_LE(0,start[1]);
        ASSERT_LE(0,start[2]);
        ASSERT_LE(start[0], 4);
        ASSERT_LE(start[1], 4);
        ASSERT_LE(start[2], 4);
        ASSERT_LE(abs(pattern[0]), 3);
        ASSERT_LE(abs(pattern[1]), 3);
        ASSERT_LE(abs(pattern[2]), 3);

        for(int i = 0; i < checklist.size(); i++) {

            if(pattern == checklist[i]) {
                checklist.erase(checklist.begin() + i);
                break;
            }
        }

        cellContainer7.setNextPath(start,pattern);
    }

    ASSERT_TRUE(checklist.empty());
}

/**
 * @brief
*/
TEST(cellcontainer, test_setNextPath6) {
    CellContainer cellContainer8{10.0, 10.0, 10.0, 5.0, 1.0};
    std::array<dim_t,3> start{0,0,0};
    std::array<dim_t,3> pattern{0,0,0};

    while(start[0] != dim_t_res) {
        ASSERT_LE(0,start[0]);
        ASSERT_LE(0,start[1]);
        ASSERT_LE(0,start[2]);
        ASSERT_LE(start[0], 10);
        ASSERT_LE(start[1], 10);
        ASSERT_LE(start[2], 10);
        ASSERT_LE(abs(pattern[0]), 5);
        ASSERT_LE(abs(pattern[1]), 5);
        ASSERT_LE(abs(pattern[2]), 5);

        cellContainer8.setNextPath(start,pattern);
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}