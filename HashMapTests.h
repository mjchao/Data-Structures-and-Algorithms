//
//  HashMapTests.h
//  Data Structures
//
//  Created by Mickey on 11/3/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__HashMapTests__
#define __Data_Structures__HashMapTests__

#include "UnitTest.h"

class HashMapTests : public UnitTest {
    
private:
    
public:
    void test();
    void testDefaultHasher();
    void testConstructors();
    void testPut();
    void testGet();
};

#endif /* defined(__Data_Structures__HashMapTests__) */
