//
//  LinkedListTests.h
//  Data Structures
//
//  Created by Mickey on 10/14/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__LinkedListTests__
#define __Data_Structures__LinkedListTests__

#include "UnitTest.h"

class LinkedListTests : public UnitTest {

private:
    
public:
    LinkedListTests();
    void test();
        void testToString();
        void testClear();
        void testInsert();
        void testInsertFront();
        void testAppend();
        void testSet();
        void testGet();
        void testContains();
        void testIndexOf();
        void testRemoveAt();
        void testRemove();
        void testSize();
        void testReferences();
        void testCopy();
        void testMemoryUsage();
};

#endif /* defined(__Data_Structures__LinkedListTests__) */
