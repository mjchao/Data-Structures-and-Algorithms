//
//  ArrayQueueTests.cpp
//  Data Structures
//
//  Created by Mickey on 10/25/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "ArrayQueue.h"
#include "ArrayQueueTests.h"

void ArrayQueueTests::test() {
    testOffer();
    testPeek();
    testPoll();
    testQueueSystem();
    reportTestStatistics( "ArrayQueue" );
}

void ArrayQueueTests::testOffer() {
    
    string expected;
    string found;
    string errorMessage = "ArrayQueue offer() failed!";
    ArrayQueue< int > test;
    
    //test empty queue
    test.clear();
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test 1 element
    test.clear();
    test.offer( 1 );
    expected = "[1]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test multiple elements
    test.clear();
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    expected = "[1, 2, 3]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void ArrayQueueTests::testPeek() {
    int expected;
    int found;
    string errorMessage = "ArrayQueue peek() failed!";
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                    "ArrayQueue peek() failed to throw proper exception!";
    
    
    ArrayQueue<int> test;
    
    //test peeking into empty queue
    test.clear();
    expectedException = test.generateEmptyQueueException();
    foundException = "No exception found.";
    try {
        test.peek();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test peeking into a quee with 1 element
    test.clear();
    test.offer( 10 );
    expected = 10;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    //test peeking into a queue with multiple elements
    test.clear();
    test.offer( 10 );
    test.offer( 11 );
    expected = 10;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
}

void ArrayQueueTests::testPoll() {

}

/**
 * Tests the offer(), peek(), and poll() functions that are
 * critical to the queue's success.
 */
void ArrayQueueTests::testQueueSystem() {
    int expected;
    int found;
    string expectedRepresentation;
    string foundRepresentation;
    string errorMessage = "ArrayQueue system failed!";
    
    ArrayQueue<int> test;
    
    //TEST CASE 1:
    //first fill up the queue so that it has array size 4 and contains 4
    //elements. this brings the tail to index 0
    test.offer( 0 );
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    evaluateTest( 0 , test.m_tailIdx , errorMessage );
    
    //then remove everything from the queue so that it has array size 4. this
    //brings the head back to index 0
    expected = 0;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expected = 1;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );

    expected = 2;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expected = 3;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    evaluateTest( 4 , test.getArraySize() , errorMessage );
    evaluateTest( 0 , test.m_headIdx , errorMessage );
    
    //then add one element to the queue, bringing the tail to 1
    test.offer( 4 );
    expectedRepresentation = "[4]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    evaluateTest( 4 , test.getArraySize() , errorMessage );
    evaluateTest( 0 , test.m_headIdx , errorMessage );
    evaluateTest( 1 , test.m_tailIdx , errorMessage );
    
    //then add another element to the queue, bringing the tail to 2
    test.offer( 5 );
    evaluateTest( 0 , test.m_headIdx , errorMessage );
    evaluateTest( 2 , test.m_tailIdx , errorMessage );
    expectedRepresentation = "[4, 5]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //TEST CASE 2
    //check that clearing the queue several times doesn't make any problems
    test.clear();
    test.clear();
    test.clear();
    
    //first fill up the queue so that it has array size 4 and contains 4
    //elements. this brings the tail to index 0
    test.offer( 0 );
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    
    //then remove all but one element from the queue. this bring the head to
    //index 3
    expected = 0;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );

    expected = 1;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expected = 2;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    evaluateTest( 3 , test.m_headIdx , errorMessage );
    
    //then add 3 elements to the queue, bringing the tail to index 3
    //although the queue is full, it should not resize just yet!
    test.offer( 10 );
    test.offer( 11 );
    test.offer( 12 );
    evaluateTest( 3 , test.m_tailIdx , errorMessage );
    expectedRepresentation = "[3, 10, 11, 12]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    evaluateTest( 4 , test.getArraySize() , errorMessage );
    
    //if we add another element, the queue should resize, and the head
    //should be reset to the 0th index, the tail set to the n+1th index,
    //which in this case is 5, since there are 5 elements in the queue now
    //(5 elements, but index 5 is the 6th element in the array)
    test.offer( 13 );
    evaluateTest( 0 , test.m_headIdx , errorMessage );
    evaluateTest( 5 , test.m_tailIdx , errorMessage );
    expectedRepresentation = "[3, 10, 11, 12, 13]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //and now, the array should have size 8, which is the next greatest power
    //of 2 from 5
    evaluateTest( 8 , test.getArraySize() , errorMessage );
    
    //TEST CASE 3
    test.clear();
    
    //every time, we add 5 elements and remove them all immediately
    //each iteration after the first, we will shift the index of the head
    //by 5. Thus, after 100 iterations, the index of the head should be at
    //(5*100) % 8 = 4
    for ( int i=0 ; i<4 ; i++ ) {
        test.offer( 10 );
        test.offer( 11 );
        test.offer( 12 );
        test.offer( 13 );
        test.offer( 14 );
        test.poll();
        test.poll();
        test.poll();
        test.poll();
        test.poll();
    }
    evaluateTest( 4 , test.m_headIdx , errorMessage );
    evaluateTest( 4 , test.m_tailIdx , errorMessage );
    
    //the size of the queue's underlying array should not have changed
    evaluateTest( 8 , test.getArraySize() , errorMessage );
}