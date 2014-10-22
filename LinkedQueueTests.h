//
//  LinkedQueueTests.h
//  Data Structures
//
//  Created by Mickey on 10/22/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__LinkedQueueTests__
#define __Data_Structures__LinkedQueueTests__

#include "UnitTest.h"

class LinkedQueueTests : public UnitTest {
    
public:
    LinkedQueueTests();
    void test();
        void testOffer();
        void testPeek();
        void testPoll();
};

#endif /* defined(__Data_Structures__LinkedQueueTests__) */
