#include <gtest/gtest.h>
#include "particleModel/storage/CellContainer.h"
#include "particleModel/storage/CellContainerIterators.h"






TEST(test_CustomIterator,test_basic){
    CellContainer container(10,10,10,2.0,2.0);
    for(auto iter = container.begin_custom(0,1,0,1,0,1); iter != container.end_custom(); ++iter){
        std::cout << "got: x=" << iter.x << " y=" << iter.y << " z=" << iter.z << "\n";
    }
}