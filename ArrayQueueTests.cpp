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
    testGetIndexInArray();
    testGet();
    testContains();
    testSet();
    testRemoveAt();
    testRemove();
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
    evaluateTest( expected , found , errorMessage );//*/
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

void ArrayQueueTests::testGet() {
    int expected;
    int found;
    string errorMessage = "ArrayQueue get() failed!";
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "ArrayQueue get() fails to throw proper exception!";
    ArrayQueue< int > test;
    
    //test getting from an empty queue
    expectedException = test.generateAccessOutOfBoundsMessage( 0 , 0 );
    foundException = "No exception found.";
    try {
        test.get( 0 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test getting from a queue with one element
    
    //first test standard queue behavior, where the head has not yet shifted
    test.clear();
    test.offer( 1 );
    expected = 1;
    found = test.get( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 1 , 1 );
    foundException = "No exception found.";
    try {
        test.get( 1 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( -1 , 1 );
    foundException = "No exception found.";
    try {
        test.get( -1 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 20 , 1 );
    foundException = "No exception found.";
    try {
        test.get( 20 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //then test getting from a queue with 1 element with the head shifted
    test.clear();
    test.offer( 0 );
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    test.poll();
    test.poll();
    test.poll();
    
    expected = 3;
    found = test.get( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 1 , 1 );
    foundException = "No exception found.";
    try {
        test.get( 1 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    
    
    //test getting from a queue with multiple elements, but head is not shifted
    test.clear();
    test.offer( 0 );
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    
    expected = 0;
    found = test.get( 0 );
    evaluateTest( expected , found , errorMessage );
    expected = 1;
    found = test.get( 1 );
    evaluateTest( expected , found , errorMessage );
    expected = 2;
    found = test.get( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 4 , 4 );
    foundException = "No exception found";
    try {
        test.get( 4 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test getting from a queue with multiple elements, but head is shifted, and
    //wrapped around
    test.clear();
    test.offer( 0 );
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    test.poll();
    test.poll();
    test.poll();
    test.offer( 0 );
    test.offer( 1 );
    test.offer( 2 );
    
    expected = 3;
    found = test.get( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 0;
    found = test.get( 1 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 1;
    found = test.get( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 2;
    found = test.get( 3 );
    evaluateTest( expected , found , errorMessage );
    
    evaluateTest( 3 , test.m_headIdx , "ArrayQueue head is off!" );
    evaluateTest( 3 , test.m_tailIdx , "ArrayQueue tail is off!" );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 4 , 4 );
    foundException = "No exception found.";
    try {
        test.get( 4 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
}

void ArrayQueueTests::testContains() {
    bool expected;
    bool found;
    string errorMessage = "ArrayQueue contains() failed!";
    ArrayQueue<int> test;
    
    //test contains on empty queue
    test.clear();
    expected = false;
    found = test.contains( 10 );
    evaluateTest( expected , found , errorMessage );
    
    //test contains on queue with 1 element
    test.clear();
    test.offer( 10 );
    expected = false;
    found = test.contains( 25 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.contains( 10 );
    evaluateTest( expected , found , errorMessage );
    
    //test contains on queue with multiple elements
    test.clear();
    test.offer( 20 );
    test.offer( 30 );
    expected = false;
    found = test.contains( 10 );
    evaluateTest( expected , found , errorMessage );
    evaluateTest( test.contains( 20 ) , true , errorMessage );
    evaluateTest( test.contains( 30 ) , true , errorMessage );
    evaluateTest( test.contains( 40 ) ,  false , errorMessage );
}

void ArrayQueueTests::testGetIndexInArray() {
    int expected;
    int found;
    string errorMessage = "ArrayQueue getArrayIndex() failed!";
    
    ArrayQueue<int> test;
    
    //use array size 16
    for ( int i=0 ; i<16 ; i++ ) {
        test.offer( i );
    }
    
    expected = 0;
    found = test.getIndexInArray( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 15;
    found = test.getIndexInArray( -1 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 8;
    found = test.getIndexInArray( 8 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 8;
    found = test.getIndexInArray( 8+16 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 8;
    found = test.getIndexInArray( 8-16 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 3;
    found = test.getIndexInArray( 3 + (16*5) );
    evaluateTest( expected , found , errorMessage );
    
    expected = 12;
    found = test.getIndexInArray( 12 - (16*12) );
    evaluateTest( expected , found , errorMessage );
    
    expected = 1;
    found = test.getIndexInArray( 1 + (16*9) );
    evaluateTest( expected , found , errorMessage );
    
    expected = 1;
    found = test.getIndexInArray( 1 - (16*16) );
    evaluateTest( expected , found , errorMessage );
}

void ArrayQueueTests::testSet() {
    string expected;
    string found;
    string errorMessage = "ArrayQueue set() failed!";
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "ArrayQueue set() fails to throw proper exception!";
    ArrayQueue<int> test;
    
    //test setting in an empty queue
    expectedException = test.generateAccessOutOfBoundsMessage( 0 , 0 );
    foundException = "No exception found";
    try {
        test.set( 0 , 999 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test setting in a queue with one element
    test.clear();
    test.offer( 0 );
    test.set( 0 , 1000 );
    expected = "[1000]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 1 , 1 );
    foundException = "No exception found";
    try {
        test.set( 1 , 9999 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test setting in a queue with multiple elements
    test.clear();
    test.offer( 0 );
    test.offer( 1 );
    test.offer( 2 );
    test.set( 0 , 10 );
    test.set( 1 , 11 );
    test.set( 2 , 12 );
    expected = "[10, 11, 12]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test setting in a queue with head shifted
    test.clear();
    test.offer( 0 );
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    test.poll();
    test.poll();
    test.poll();
    test.offer( 4 );
    test.offer( 5 );
    test.offer( 6 );
    test.set( 0 , 13 );
    test.set( 1 , 14 );
    test.set( 2 , 15 );
    test.set( 3 , 16 );
    expected = "[13, 14, 15, 16]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test setting in a queue with one element with head shifted
    test.clear();
    test.offer( 0 );
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    test.poll();
    test.poll();
    test.poll();
    test.set( 0 , 999 );
    expected = "[999]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 1 , 1 );
    foundException = "No exception found.";
    try {
        test.set( 1 , 1 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
}

void ArrayQueueTests::testRemoveAt() {
    string expected;
    string found;
    string errorMessage;
    string expectedException;
    string foundException;
    string exceptionErrorMessage;
    
    ArrayQueue<int> test;
    
    //test removing from an empty queue
    test.clear();
    expectedException = test.generateAccessOutOfBoundsMessage( 0 , 0 );
    foundException = "No exception found";
    try {
        test.removeAt( 0 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test removing from an queue with non-shifted head
    test.clear();
    test.offer( 10 );
    test.offer( 11 );
    test.offer( 12 );
    test.offer( 13 );
    
    test.removeAt( 3 );
    test.removeAt( 2 );
    expected = "[10, 11]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );

    test.removeAt( 0 );
    expected = "[11]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 0 );
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test removing from a queue with shifted head
    test.clear();
    test.offer( 10 );
    test.offer( 11 );
    test.offer( 12 );
    test.offer( 13 );
    test.poll();
    test.poll();
    test.poll();
    test.offer( 10 );
    test.offer( 11 );
    test.offer( 12 );
    
    test.removeAt( 0 );
    expected = "[10, 11, 12]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 3 , 3 );
    foundException = "No exception found.";
    try {
        test.removeAt( 3 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    test.removeAt( 2 );
    expected = "[10, 11]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 1 );
    expected = "[10]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void ArrayQueueTests::testRemove() {
    string expected;
    string found;
    string errorMessage = "ArrayQueue remove() failed!";
    
    
    ArrayQueue<int> test;
    
    //test removing from empty list
    test.clear();
    evaluateTest( test.remove( 1 ) , false , errorMessage );
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test removing from queue with 1 element
    test.clear();
    test.offer( 10 );
    evaluateTest( test.remove( 10 ) , true , errorMessage );
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test removing from queue with 2 elements
    test.clear();
    test.offer( 10 );
    test.offer( 20 );
    evaluateTest( test.remove( 20 ) , true , errorMessage );
    expected = "[10]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 20 );
    evaluateTest( test.remove( 10 ) , true , errorMessage );
    
    expected = "[20]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test removing from queue with shifted head and with multiple elements
    test.clear();
    test.offer( 10 );
    test.offer( 20 );
    test.offer( 30 );
    test.poll();
    test.poll();
    test.poll();
    
    test.offer( 40 );
    test.offer( 50 );
    test.offer( 60 );
    
    evaluateTest( test.remove( 50 ) , true , errorMessage );
    expected = "[40, 60]";
    found = test.toString();
    
    test.offer( 70 );
    test.offer( 80 );
    evaluateTest( test.remove( 70 ) , true , errorMessage );
    expected = "[40, 60, 80]";
    found = test.toString();
    
}
