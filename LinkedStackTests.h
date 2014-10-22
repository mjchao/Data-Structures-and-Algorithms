//
//  LinkedStackTests.h
//  Data Structures
//
//  Created by Mickey on 10/22/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__LinkedStackTests__
#define __Data_Structures__LinkedStackTests__

#include "LinkedStack.h"

class LinkedStackTests : public UnitTest {
    
public:
    LinkedStackTests();
    void test();
        void testPush();
        void testPeek();
        void testPop();
};
#endif /* defined(__Data_Structures__LinkedStackTests__) */
