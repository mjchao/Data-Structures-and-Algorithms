//
//  main.cpp
//  Data Structures
//
//  Created by Mickey on 9/1/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include <iostream>
#include "ArrayListTests.h"
#include "LinkedListTests.h"
#include "LinkedStackTests.h"
#include "LinkedQueueTests.h"
#include "ArrayQueueTests.h"
#include "HashmapTests.h"
#include "PriorityQueueTests.h"

#include "UnitTest.h"
#include <vector>
using std::vector;

using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {
    // insert code here...
    ArrayListTests test;
    //test.test();
    
    LinkedListTests test2;
    //test2.test();
    
    LinkedStackTests test3;
    //test3.test();
    
    LinkedQueueTests test4;
    //test4.test();
    
    ArrayQueueTests test5;
    //test5.test();

    HashmapTests test6;
    //test6.test();
    
    PriorityQueueTests test7;
    test7.test();
    return 0;
}
