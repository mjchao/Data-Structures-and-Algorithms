//
//  LinkedListTests.cpp
//  Data Structures
//
//  Created by Mickey on 10/14/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "LinkedListTests.h"
#include "LinkedList.h"

using std::tolower;
using std::runtime_error;

LinkedListTests::LinkedListTests() {
    
}

void LinkedListTests::test() {
    testToString();
    testClear();
    testInsert();
    //testMemoryUsage();
    reportTestStatistics( "LinkedList" );
}

void LinkedListTests::testToString() {
    string errorMessage = "LinkedList toString() failed!";
    string expected;
    string found;
    
    LinkedList< int > test;
    LinkedList<int>::Link first;
    first.value = 1000;
    
    LinkedList<int>::Link second;
    second.value = 500;
    
    first.next = &second;
    second.prev = &first;
    
    test.m_head = &first;
    test.m_numElements = 2;
    
    //test that toString() does not alter elements
    expected = "[1000, 500]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test that toString() does not alter links
    first.value = 2000;
    second.value = 750;
    expected = "[2000, 750]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test that we can still insert after toString() is called
    LinkedList<int>::Link third;
    third.value = 250;
    second.next = &third;
    third.prev = &second;
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
    
    test.insert( 1 , 5 );
    expected = "[2, 5, 1000, 1]";
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
    
    test.clear();
    test.insert( 0 , 10 );
    expected = 10;
    found = test.get( 0 );
    evaluateTest( expected , found , errorMessage );
    test.insert( 1 , 11 );
    test.insert( 2 , 12 );
    expected = 11;
    
}

void LinkedListTests::testMemoryUsage() {
    char expected;
    char found;
    string errorMessage = "LinkedList memory does not free memory correctly!";
    cout << "Prepare to observe memory usage. Press enter to continue" << endl;
    getchar();
    LinkedList<int> test;
    for ( int i=0 ; i<30000000 ; i++ ) {
        test.insert( 0 , 1 );
        test.insert( 0 , 2 );
        test.insert( 0 , 3 );
        test.clear();
    }
    cout << "Was memory usage constant? (Y/N)" << endl;
    found = tolower( getchar() );
    expected = 'y';
    evaluateTest( expected , found , errorMessage );
}