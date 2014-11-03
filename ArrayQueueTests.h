//
//  ArrayQueueTests.h
//  Data Structures
//
//  Created by Mickey on 10/25/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__ArrayQueueTests__
#define __Data_Structures__ArrayQueueTests__

#include "UnitTest.h"

class ArrayQueueTests : public UnitTest {
    
public:
    void test();
        void testOffer();
        void testPeek();
        void testPoll();
        void testQueueSystem();
        void testGetIndexInArray();
        void testGet();
        void testSet();
        void testRemoveAt();
        void testRemove();
};

#endif /* defined(__Data_Structures__ArrayQueueTests__) */
