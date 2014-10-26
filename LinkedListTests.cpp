//
//  LinkedListTests.cpp
//  Data Structures
//
//  Created by Mickey on 10/14/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "LinkedListTests.h"
#include "LinkedList.h"
#include "ArrayList.h"
#include <vector>
    using std::vector;

using std::tolower;
using std::runtime_error;

LinkedListTests::LinkedListTests() {
    
}

void LinkedListTests::test() {
    testToString();
    testClear();
    testInsert();
    testInsertFront();
    testAppend();
    testSet();
    testGet();
    testContains();
    testIndexOf();
    testRemoveAt();
    testRemove();
    testSize();
    testReferences();
    //testMemoryUsage();
    testCopy();
    reportTestStatistics( "LinkedList" );
}

void LinkedListTests::testToString() {
    string errorMessage = "LinkedList toString() failed!";
    string expected;
    string found;
    
    LinkedList< int > test;
    LinkedList<int>::Link* first = new LinkedList<int>::Link;
    (*first).value = 1000;
    (*first).prev = 0;
    
    LinkedList<int>::Link* second = new LinkedList<int>::Link;
    (*second).value = 500;
    
    (*first).next = second;
    (*second).prev = first;
    
    test.m_head = first;
    test.m_numElements = 2;
    test.m_tail = second;
    
    //test that toString() does not alter elements
    expected = "[1000, 500]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test that toString() does not alter links
    (*first).value = 2000;
    (*second).value = 750;
    expected = "[2000, 750]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test that we can still insert after toString() is called
    LinkedList<int>::Link* third = new LinkedList<int>::Link;
    (*third).value = 250;
    (*third).next = 0;
    (*second).next = third;
    (*third).prev = second;
    test.m_tail = third;
    test.m_numElements = 3;
    expected = "[2000, 750, 250]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testClear() {
    string expected;
    string found;
    string errorMessage = "LinkedList clear() failed!";
    
    LinkedList<int> test;
    //test clearing an empty list
    test.clear();
    test.clear();
    test.clear();
    
    //test clearing a list with elements
    LinkedList<int>::Link* first = new LinkedList<int>::Link;
    LinkedList<int>::Link* second = new LinkedList<int>::Link;
    LinkedList<int>::Link* third = new LinkedList<int>::Link;
    
    (*first).value = 1000;
    (*first).prev = 0;
    (*first).next = second;

    (*second).value = 2000;
    (*second).prev = first;
    (*second).next = third;
    
    (*third).value = 3000;
    (*third).prev = second;
    (*third).next = 0;
    
    test.m_head = first;
    test.m_tail = third;
    test.m_numElements = 3;
    test.clear();
    
    //test for representation
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test that the head is gone
    LinkedList<int>::Link* expectedHead = 0;
    LinkedList<int>::Link* foundHead = test.m_head;
    evaluateTest( expectedHead , foundHead , errorMessage );
    
    //test that the tail is gone
    LinkedList<int>::Link* expectedTail = 0;
    LinkedList<int>::Link* foundTail = test.m_tail;
    evaluateTest( expectedTail , foundTail , errorMessage );
    
    //test that you can still insert after clearing
    test.insert( 0 , 0 );
    test.insert( 1 , 1 );
    test.insert( 2 , 2 );
    expected = "[0, 1, 2]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testInsert() {
    string expected;
    string found;
    string errorMessage = "LinkedList insert() failed!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                                "LinkedList fails to throw exception properly!";
    LinkedList< int > test;
    
    test.insert( 0 , 1000 );
    expected = "[1000]";
    found = test.toString();
    evaluateTest( expected, found , errorMessage );
    
    test.insert( 0 , 1 );
    expected = "[1, 1000]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.clear();
    test.insert( 0 , 1000 );
    test.insert( 1 , 1 );
    expected = "[1000, 1]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 0 , 2 );
    expected = "[2, 1000, 1]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test inserting in first half and second half of list
    test.insert( 1 , 5 );
    expected = "[2, 5, 1000, 1]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 3 , 333 );
    expected = "[2, 5, 1000, 333, 1]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test insert index too high on empty list
    test.clear();
    expectedException = test.generateInsertOutOfBoundsErrorMessage( 1 , 0 );
    foundException = "No exception found";
    try {
        test.insert( 1 , 1 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test insert negative index on empty list
    test.clear();
    expectedException = test.generateInsertOutOfBoundsErrorMessage( -1 , 0 );
    foundException = "No exception found";
    try {
        test.insert( -1 , 1 );
    }
    catch ( std::runtime_error e  ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test insert out of bounds on list with 1 element
    test.clear();
    test.insert( 0 , 0 );
    expectedException = test.generateInsertOutOfBoundsErrorMessage( 2 , 1 );
    foundException = "No exception found";
    try {
        test.insert( 2 , 2 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    expectedException = test.generateInsertOutOfBoundsErrorMessage( -1 , 1 );
    foundException = "No exception found";
    try {
        test.insert( -1 , 2 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
}

void LinkedListTests::testInsertFront() {
    string expected;
    string found;
    string errorMessage = "LinkedList insert() at front failed!";
    
    LinkedList<int> test;
    test.insert( 0 );
    expected = "[0]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 1 );
    expected = "[1, 0]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 2 );
    expected = "[2, 1, 0]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testAppend() {
    string expected;
    string found;
    string errorMessage = "LinkedList append() failed!";
    
    LinkedList<int> test;
    
    //test empty list
    test.clear();
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test appending to empty list
    test.clear();
    test.append( 20 );
    expected = "[20]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test appending multiple value
    test.clear();
    test.append( 20 );
    test.append( 21 );
    test.append( 22 );
    test.append( 23 );
    test.append( 24 );
    expected = "[20, 21, 22, 23, 24]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testSet() {
    string expected;
    string found;
    string errorMessage = "LinkedList set() failed!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "LinkedList set() failed to throw proper exception!";
    
    LinkedList<string> test;
    
    //test setting in an empty list
    test.clear();
    expectedException = test.generateAccessOutOfBoundsErrorMessage( 0 , 0 );
    foundException = "No exception found";
    try {
        test.set( 0 , "hi" );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test setting in a list with 1 element
    test.clear();
    test.append( "hello" );
    test.set( 0 , "hi" );
    
    expected = "[hi]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsErrorMessage( 1 , 1 );
    foundException = "No exception found";
    try {
        test.set( 1 , "hello" );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    
    //test redundant set
    test.clear();
    test.append( "hello" );
    test.set( 0 , "hi" );
    test.set( 0 , "hi" );
    expected = "[hi]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test setting in a list with multiple elements
    test.clear();
    test.append( "0" );
    test.append( "1" );
    test.append( "2" );
    test.append( "3" );
    test.append( "4" );
    test.append( "5" );
    
    test.set( 0 , "10" );
    expected = "[10, 1, 2, 3, 4, 5]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.set( 1 , "11" );
    expected = "[10, 11, 2, 3, 4, 5]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.set( 2 , "12" );
    test.set( 3 , "13" );
    test.set( 4 , "14" );
    test.set( 5 , "15" );
    expected = "[10, 11, 12, 13, 14, 15]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsErrorMessage( 6 , 6 );
    foundException = "No exception found";
    try {
        test.set( 6 , "16" );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
}

void LinkedListTests::testGet() {
    int expected;
    int found;
    string errorMessage = "LinkedList get() failed!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "LinkedList get() fails to throw propert exception!";
    
    LinkedList<int> test;
    expectedException = test.generateAccessOutOfBoundsErrorMessage( 0 , 0 );
    foundException = "No exception found.";
    try {
        test.get( 0 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test getting from a list with 1 element
    test.clear();
    test.insert( 0 , 10 );
    expected = 10;
    found = test.get( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsErrorMessage( 1 , 1 );
    foundException = "No exception found.";
    try {
        test.get( 1 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    expectedException = test.generateAccessOutOfBoundsErrorMessage( -1 , 1 );
    foundException = "No exception found.";
    try {
        test.get( -1 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test getting from a list with multiple elements
    test.clear();
    test.insert( 0 , 10 );
    test.insert( 1 , 11 );
    test.insert( 2 , 12 );
    test.insert( 3 , 13 );
    test.insert( 4 , 14 );
    test.insert( 5 , 15 );
    expected = 10;
    found = test.get( 0 );
    evaluateTest( expected , found , errorMessage );
    expected = 11;
    found = test.get( 1 );
    evaluateTest( expected , found , errorMessage );
    expected = 12;
    found = test.get( 2 );
    evaluateTest( expected , found , errorMessage );
    expected = 13;
    found = test.get( 3 );
    evaluateTest( expected , found , errorMessage );
    expected = 14;
    found = test.get( 4 );
    evaluateTest( expected , found , errorMessage );
    expected = 15;
    found = test.get( 5 );
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testContains() {
    bool expected;
    bool found;
    string errorMessage = "LinkedList contains() failed!";
    
    LinkedList<int> test;
    
    //test empty list
    expected = false;
    found = test.contains( 0 );
    evaluateTest( expected , found , errorMessage );
    
    //test list with 1 element
    test.clear();
    test.append( 0 );
    expected = true;
    found = test.contains( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expected = false;
    found = test.contains( 1 );
    evaluateTest( expected , found , errorMessage );
    
    //test list with multiple elements
    test.append( 0 );
    test.append( 1 );
    test.append( 2 );
    test.append( 3 );
    test.append( 4 );
    test.append( 5 );
    
    expected = true;
    found = test.contains( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.contains( 1 );
    evaluateTest( expected , found , errorMessage );
    
    expected = false;
    found = test.contains( -1000 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.contains( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.contains( 3 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.contains( 4 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.contains( 5 );
    evaluateTest( expected , found , errorMessage );
    
}

void LinkedListTests::testIndexOf() {
    int expected;
    int found;
    string errorMessage = "LinkedList indexOf() failed!";
    
    LinkedList<int> test;
    
    //test on empty list
    test.clear();
    expected = -1;
    found = test.indexOf( 10 );
    evaluateTest( expected , found, errorMessage );
    
    //test on list with 1 element
    test.clear();
    test.append( 10 );
    expected = 0;
    found = test.indexOf( 10 );
    evaluateTest( expected , found , errorMessage );
    
    expected = -1;
    found = test.indexOf( 1000 );
    evaluateTest( expected , found , errorMessage );
    
    //test on list with multiple elements
    test.clear();
    test.append( 10 );
    test.append( 11 );
    test.append( 12 );
    test.append( 13 );
    test.append( 14 );
    expected = 0;
    found = test.indexOf( 10 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 1;
    found = test.indexOf( 11 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 2;
    found = test.indexOf( 12 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 3;
    found = test.indexOf( 13 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 4;
    found = test.indexOf( 14 );
    evaluateTest( expected , found , errorMessage );
    
    expected = -1;
    found = test.indexOf( 15 );
    evaluateTest( expected , found , errorMessage );
    
    expected = -1;
    found = test.indexOf( -14 );
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testRemoveAt() {
    string expected;
    string found;
    string errorMessage = "LinkedList removeAt() failed!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                    "LinkedList removeAt() fails to throw proper exception!";
    
    LinkedList<string> test;
    
    //test removing from empty list
    test.clear();
    expectedException = test.generateAccessOutOfBoundsErrorMessage( 0 , 0 );
    foundException = "No exception found";
    try {
        test.removeAt( 0 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test removing from a list with 1 element
    test.clear();
    test.append( "zero" );
    
    expectedException = test.generateAccessOutOfBoundsErrorMessage( 1 , 1 );
    foundException = "No exception found";
    try {
        test.removeAt( 1 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    expected = "zero";
    found = test.removeAt( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //but make sure we can add more elements after removing the head
    test.append( "zero" );
    test.append( "one" );
    test.append( "two" );
    expected = "[zero, one, two]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test removing just the head from a list of multiple elements
    test.clear();
    test.append( "zero" );
    test.append( "one" );
    test.append( "two" );
    test.append( "three" );
    test.append( "four" );
    
    test.removeAt( 0 );
    expected = "[one, two, three, four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 0 );
    expected = "[two, three, four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 0 );
    test.removeAt( 0 );
    expected = "[four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 0 );
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test removing just the tail from a list of multiple elements
    test.clear();
    test.append( "zero" );
    test.append( "one" );
    test.append( "two" );
    test.append( "three" );
    test.append( "four" );
    
    expected = "four";
    found = test.removeAt( 4 );
    evaluateTest( expected , found , errorMessage );
    expected = "[zero, one, two, three]";
    found = test.toString();
    evaluateTest( expected, found , errorMessage );
    
    test.removeAt( 3 );
    expected = "[zero, one, two]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 2 );
    expected = "[zero, one]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 1 );
    expected = "[zero]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 0 );
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test removing from the center of a list of multiple elements
    test.clear();
    test.append( "zero" );
    test.append( "one" );
    test.append( "two" );
    test.append( "three" );
    test.append( "four" );
    test.append( "five" );
    
    test.removeAt( 3 );
    expected = "[zero, one, two, four, five]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 2 );
    expected = "[zero, one, four, five]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 1 );
    expected = "[zero, four, five]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsErrorMessage( 3 , 3 );
    foundException = "No exception found";
    try {
        test.removeAt( 3 );
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    test.removeAt( 1 );
    expected = "[zero, five]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 1 );
    expected = "[zero]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 0 );
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testRemove() {
    string expected;
    string found;
    bool expectedRtn;
    bool foundRtn;
    string errorMessage = "LinkedList remove() failed!";
    
    //test removing from empty list
    LinkedList<string> test;
    test.clear();
    expectedRtn = false;
    foundRtn = test.remove( "" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    //test removing from list with 1 element
    test.clear();
    test.append( "one" );
    
    expectedRtn = false;
    foundRtn = test.remove( "1" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "one" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[]";
    found = test.toString();
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    //test removing from end of list with multiple elements
    test.clear();
    test.append( "one" );
    test.append( "two" );
    test.append( "three" );
    test.append( "four" );
    test.append( "five" );
    
    expectedRtn = true;
    foundRtn = test.remove( "five" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[one, two, three, four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "four" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    //check that a failed remove does not affect future removes
    expectedRtn = false;
    foundRtn = test.remove( "3" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[one, two, three]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expected = "[one, two, three]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "three" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[one, two]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "two" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[one]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "one" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test removing from start of list with multiple elements
    test.clear();
    test.append( "one" );
    test.append( "two" );
    test.append( "three" );
    test.append( "four" );
    
    expectedRtn = true;
    foundRtn = test.remove( "one" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[two, three, four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "two" );
    evaluateTest( expected , found , errorMessage );
    
    expected = "[three, four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = false;
    foundRtn = test.remove( "3" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[three, four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "three" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "four" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test removing from middle of list with multiple elements
    test.clear();
    test.append( "zero" );
    test.append( "one" );
    test.append( "two" );
    test.append( "three" );
    test.append( "four" );
    
    expectedRtn = true;
    foundRtn = test.remove( "two" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[zero, one, three, four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "three" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[zero, one, four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedRtn = true;
    foundRtn = test.remove( "one" );
    evaluateTest( expectedRtn , foundRtn , errorMessage );
    
    expected = "[zero, four]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //no need to test further removal, as this has degenerated into removing
    //from start and end of list
}

void LinkedListTests::testSize() {
    int expected;
    int found;
    string errorMessage = "LinkedList does not maintain proper size!";
    
    LinkedList<int> test;
    test.append( 0 );
    expected = 1;
    found = test.size();
    evaluateTest( expected , found , errorMessage );
    
    test.remove( 0 );
    expected = 0;
    found = test.size();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 0 );
    expected = 1;
    found = test.size();
    evaluateTest( expected , found , errorMessage );
    
    test.removeAt( 0 );
    expected = 0;
    found = test.size();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 0 , 0 );
    test.insert( 1 , 1 );
    expected = 2;
    found = test.size();
    evaluateTest( expected , found , errorMessage );
    
    test.clear();
    expected = 0;
    found = test.size();
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testReferences() {
    
    string expected;
    string found;
    string errorMessage = "LinkedList fails to handle references properly!";
    
    LinkedList<int> test;
    
    test.append( 0 );
    test.append( 1 );
    test.append( 2 );
    test.get( 0 ) = 5;
    
    expected = "[5, 1, 2]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testMemoryUsage() {
    char expected;
    char found;
    string errorMessage = "LinkedList memory does not free memory correctly!";
    cout << "Prepare to observe memory usage. Press enter to continue" << endl;
    getchar();
    LinkedList<int> test;
    for ( int i=0 ; i<10000000 ; i++ ) {
        test.insert( 0 , 1 );
        test.insert( 0 , 2 );
        test.insert( 0 , 3 );
        test.clear();
    }
    for ( int i=0 ; i<10000000 ; i++ ) {
        test.append( 0 );
        test.append( 1 );
        test.append( 2 );
        test.remove( 0 );
        test.remove( 1 );
        test.remove( 2 );
    }
    for ( int i=0 ; i<10000000 ; i++ ) {
        test.append( 0 );
        test.append( 1 );
        test.append( 2 );
        test.remove( 2 );
        test.remove( 1 );
        test.remove( 0 );
    }
    
    //test deconstructor
    for ( int i=0 ; i<10000000 ; i++ ) {
        LinkedList<int>* test2 = new LinkedList<int>;
        test2->append( 0 );
        test2->append( 1 );
        test2->append( 2 );
        delete test2;
    }
    cout << "Was memory usage constant? (Y/N)" << endl;
    found = tolower( getchar() );
    expected = 'y';
    evaluateTest( expected , found , errorMessage );
}

void LinkedListTests::testCopy() {
    string errorMessage = "LinkedList assignment operator failed!";
    LinkedList<int> test;
    test.append( 0 );
    test.append( 1 );
    test.append( 2 );
    
    LinkedList<int> copy;
    copy = test;
    evaluateTest( test.get( 0 ) , copy.get( 0 ) , errorMessage );
    evaluateTest( test.get( 0 ), copy.get( 0 ) , errorMessage );
    evaluateTest( test.get( 0 ) , copy.get( 0 ) , errorMessage );
}