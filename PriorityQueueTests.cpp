//
//  PriorityQueueTests.cpp
//  Data Structures
//
//  Created by Mickey on 11/4/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "PriorityQueueTests.h"
#include "PriorityQueue.h"

void PriorityQueueTests::test() {
    testOffer();
    testQueueSystem();
    testRemove();
    reportTestStatistics( "PriorityQueue" );
}

void PriorityQueueTests::testOffer() {
    string expected;
    string found;
    string errorMessage = "PriorityQueue offer() failed!";
    PriorityQueue<int> test;
    
    //test empty
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test in order insertion
    test.clear();
    test.offer( 1 );
    expected = "[1]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 2 );
    expected = "[1, 2]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 3 );
    expected = "[1, 2, 3]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test partially in order insertion
    test.clear();
    test.offer( 10 );
    expected = "[10]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 1 );
    expected = "[1, 10]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 2 );
    expected = "[1, 10, 2]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 3 );
    expected = "[1, 3, 2, 10]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test reverse order insertion
    test.clear();
    test.offer( 3 );
    expected = "[3]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 2 );
    expected = "[2, 3]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 1 );
    expected = "[1, 3, 2]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test mixed order insertion
    test.clear();
    test.offer( 50 );
    expected = "[50]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 100 );
    expected = "[50, 100]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 25 );
    expected = "[25, 100, 50]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 75 );
    expected = "[25, 75, 50, 100]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( 0 );
    expected = "[0, 25, 50, 100, 75]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void PriorityQueueTests::testQueueSystem() {
    int expected;
    int found;
    string errorMessage = "PriorityQueue poll() failed!";
    
    string expectedRep;
    string foundRep;
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                "PriorityQueue poll() failed to throw proper exception!";
    
    PriorityQueue<int> test;
    
    //test in order insertion
    test.clear();
    test.offer( 1 );
    expected = 1;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    test.clear();
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    expected = 1;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    expected = 2;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    expected = 3;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    //test reverse order insertion
    test.clear();
    test.offer( 30 );
    test.offer( 20 );
    test.offer( 10 );
    expected = 10;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    expected = 20;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    expected = 30;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateEmptyQueueException();
    foundException = "No exception found.";
    try {
        test.poll();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test mixed order insertion
    test.clear();
    test.offer( 50 );
    test.offer( 100 );
    test.offer( 25 );
    test.offer( 75 );
    test.offer( 0 );
    expectedRep = "[0, 25, 50, 100, 75]";
    foundRep = test.toString();
    evaluateTest( expectedRep , foundRep , errorMessage );
    
    expected = 0;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    test.poll();

    expectedRep = "[25, 75, 50, 100]";
    foundRep = test.toString();
    evaluateTest( expectedRep , foundRep , errorMessage );
    
    expected = 25;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    test.poll();
    
    expectedRep = "[50, 75, 100]";
    foundRep = test.toString();
    evaluateTest( expectedRep , foundRep , errorMessage );
    
    expected = 50;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    test.poll();
    
    expected = 75;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    test.poll();
    
    expected = 100;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    test.poll();
}

void PriorityQueueTests::testRemove() {
    
    bool expected;
    bool found;
    string expectedRepresentation;
    string foundRepresentation;
    string errorMessage = "PriorityQueue remove() failed!";
    
    PriorityQueue< int > test;

    //test removing from the front (equivalent to polling)
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    test.offer( 4 );
    test.offer( 5 );
    
    expectedRepresentation = "[1, 2, 3, 4, 5]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 1 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[2, 4, 3, 5]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = false;
    found = test.remove( -1 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[2, 4, 3, 5]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[3, 4, 5]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = false;
    found = test.remove( 1 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.remove( 3 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[4, 5]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 4 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[5]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 5 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //test removing from an empty queue
    expected = false;
    found = test.remove( 5 );
    evaluateTest( expected , found , errorMessage );
    
    //test removing from the back
    test.clear();
    test.offer( 1 );
    test.offer( 2 );
    test.offer( 3 );
    test.offer( 4 );
    test.offer( 5 );
    test.offer( 6 );
    expectedRepresentation = "[1, 2, 3, 4, 5, 6]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 6 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1, 2, 3, 4, 5]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 5 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1, 2, 3, 4]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 4 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1, 2, 3]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = false;
    found = test.remove( 4 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.remove( 3 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1, 2]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = false;
    found = test.remove( -1 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.remove( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 1 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //test removing from the middle
    test.clear();
    for ( int i=1 ; i<=4 ; i++ ) {
        test.offer( i );
    }
    
    expectedRepresentation = "[1, 2, 3, 4]";
    foundRepresentation = test.toString();
    
    expected = true;
    found = test.remove( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1, 4, 3]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 3 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1, 4]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = false;
    found = test.remove( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.remove( 1 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[4]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 4 );
    evaluateTest( expected , found , errorMessage );
    
    expected = false;
    found = test.remove( 4 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
}