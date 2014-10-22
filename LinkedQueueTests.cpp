//
//  LinkedQueueTests.cpp
//  Data Structures
//
//  Created by Mickey on 10/22/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "LinkedQueueTests.h"
#include "LinkedQueue.h"

LinkedQueueTests::LinkedQueueTests() {
    
}

void LinkedQueueTests::test() {
    testOffer();
    testPeek();
    testPoll();
    reportTestStatistics( "LinkedQueue" );
}

void LinkedQueueTests::testOffer() {
    string expected;
    string found;
    string errorMessage = "LinkedQueue offer() failed!";
    
    LinkedQueue<string> test;
    
    test.offer( "zero" );
    expected = "[zero]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( "one" );
    expected = "[zero, one]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( "two" );
    expected = "[zero, one, two]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
}

void LinkedQueueTests::testPeek() {
    string expected;
    string found;
    string errorMessage = "LinkedQueue peek() failed!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "LinkedQueue peek() fails to throw proper exception!";
    
    LinkedQueue<string> test;
    
    //test peeking into empty queue
    test.clear();
    expectedException = test.generateEmptyQueueErrorMessage();
    foundException = "No exception found.";
    try {
        test.peek();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test peeking into queue with 1 element
    test.clear();
    test.offer( "zero" );
    
    expected = "zero";
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    //test peeking into queue with multiple elements
    test.clear();
    test.offer( "zero" );
    test.offer( "one" );
    test.offer( "two" );
    expected = "zero";
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    test.offer( "three" );
    expected = "zero";
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    test.poll();
    expected = "one";
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    test.poll();
    expected = "two";
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    test.poll();
    expected = "three";
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    test.poll();
    expectedException = test.generateEmptyQueueErrorMessage();
    foundException = "No exception found.";
    try {
        test.peek();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
}

void LinkedQueueTests::testPoll() {
    int expected;
    int found;
    string errorMessage = "LinkedQueue poll() failed!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "LinkedQueue poll() fails to throw proper exception!";
    
    LinkedQueue<int> test;
    
    //test polling from empty queue
    test.clear();
    expectedException = test.generateEmptyQueueErrorMessage();
    foundException = "No exception found.";
    try {
        test.poll();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test polling from queue with 1 element
    test.clear();
    test.offer( 25 );
    expected = 25;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateEmptyQueueErrorMessage();
    foundException = "No exception found.";
    try {
        test.poll();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test polling from queue with multiple elements
    test.clear();
    test.offer( 10 );
    test.offer( 11 );
    test.offer( 12 );
    test.offer( 13 );
    
    expected = 10;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expected = 11;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expected = 12;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expected = 13;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateEmptyQueueErrorMessage();
    foundException = "No exception found.";
    try {
        test.poll();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    test.offer( 20 );
    test.offer( 21 );
    
    expected = 20;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expected = 21;
    found = test.poll();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateEmptyQueueErrorMessage();
    foundException = "No exception found.";
    try {
        test.poll();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
}