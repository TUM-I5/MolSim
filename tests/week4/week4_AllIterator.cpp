#include <gtest/gtest.h>
#include "particleModel/storage/CellContainer.h"
#include "particleModel/storage/CellContainerIterators.h"

#include "particleModel/updating/CellCalculator.h"







TEST(test_all_iterator,test_basic){
    CellContainer cellContainer(2,2,2,1.0,1.0);

    std::cout << cellContainer.to_string() << std::endl;

    for(auto iter = cellContainer.begin_all(); iter != cellContainer.end_all(); ++iter){
        std::cout << "x= " << iter.x << " y = " << iter.y << " z=" << iter.z << std::endl;
    }


}