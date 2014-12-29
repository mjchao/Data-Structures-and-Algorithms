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
#include "HashMapTests.h"
#include "PriorityQueueTests.h"
#include "BPTreeTests.h"

#include "UnitTest.h"
#include <vector>
using std::vector;

using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {
    // insert code here...
    ArrayListTests test;
    test.test();
    
    LinkedListTests test2;
    //test2.test();
    
    LinkedStackTests test3;
    //test3.test();
    
    LinkedQueueTests test4;
    //test4.test();
    
    ArrayQueueTests test5;
    //test5.test();
    
    PriorityQueueTests test6;
    //test6.test();
    
    HashMapTests test7;
    //test7.test();
    
    BPTreeTests test8;
    //test8.test();
    return 0;
}
