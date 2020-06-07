#include <iostream>

#include "B+ Tree/BPlusTreeNode.h"
#include "Testing/Testing.h"

int main(int argc, char * argv[])
{
    auto unittests = UnitTests();

    //unittests.testSingleInsertion_RootShouldOnlyContainOneValue();
    //unittests.testThreeInsert_ShouldContainThreeValues_OnReturnOf123SearchKeyAndRecordId();
    //unittests.testThreeInputsOutOfOrder_ShouldContainThreeValues_OnReturOf123SearchKeyAndRecordId();
    unittests.testFiveInputsInOrder_ShouldContainValues12345ForSearchKeyAndRecordId();
}