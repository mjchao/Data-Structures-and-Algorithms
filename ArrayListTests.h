//
//  ListTests.h
//  Data Structures
//
//  Created by Mickey on 10/11/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__ListTests__
#define __Data_Structures__ListTests__

#include "UnitTest.h"

class ArrayListTests : public UnitTest {
    
public:
    ArrayListTests();
    ~ArrayListTests();
    
    void test();
        void testArrayListAppend();
        void testArrayListInsert();
        void testArrayListClear();
        void testArrayListInsertAt();
        void testArrayListSet();
        void testArrayListGet();
        void testArrayListContains();
        void testArrayListIndexOf();
        void testArrayListRemoveAt();
        void testArrayListRemove();
        void testArrayListResize();
        void testArrayListMemoryUsage();
        void systemTest();
};
#endif /* defined(__Data_Structures__ListTests__) */
