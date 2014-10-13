//
//  ArrayListTests.cpp
//  Data Structures
//
//  Created by Mickey on 10/11/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include <iostream>
    using std::cout;
    using std::endl;

#include <string>
    using std::string;

using std::runtime_error;
using std::to_string;

#include "UnitTest.h"
#include "ArrayListTests.h"
#include "ArrayList.h"

ArrayList<int> reverseList( ArrayList<int> l );

ArrayListTests::ArrayListTests() {
    
}

ArrayListTests::~ArrayListTests() {
    
}

void ArrayListTests::test() {
    testArrayListAppend();
    testArrayListInsert();
    testArrayListClear();
    testArrayListInsertAt();
    testArrayListGet();
    testArrayListContains();
    testArrayListRemoveAt();
    testArrayListRemove();
    testArrayListResize();
    testArrayListSet();
    //testArrayListMemoryUsage();
    systemTest();
    UnitTest::test();
}

void ArrayListTests::testArrayListAppend() {
    string expected;
    string errorMessage = "ArrayList append() failed!";
    ArrayList<int> test;
    
    //test appending just 1 element
    test.append( 0 );
    expected = "[0]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test appending an odd number of elements
    test.clear();
    test.append( 1 );
    test.append( 2 );
    test.append( 3 );
    test.append( 4 );
    test.append( 5 );
    expected = "[1, 2, 3, 4, 5]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test appending an even number of elements
    test.clear();
    test.append( 1 );
    test.append( 2 );
    test.append( 3 );
    test.append( 4 );
    test.append( 5 );
    test.append( 6 );
    expected = "[1, 2, 3, 4, 5, 6]";
    evaluateTest( expected , test.toString() , errorMessage );
}

void ArrayListTests::testArrayListInsert() {
    string expected;
    string errorMessage = "ArrayList insert() failed!";
    ArrayList<int> test;
    
    //test inserting just 1 element
    test.insert( 0 );
    expected = "[0]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test inserting an odd number of elements
    test.clear();
    test.insert( 1 );
    test.insert( 2 );
    test.insert( 3 );
    test.insert( 4 );
    test.insert( 5 );
    expected = "[1, 2, 3, 4, 5]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test inserting an even number of elements
    test.clear();
    test.insert( 1 );
    test.insert( 2 );
    test.insert( 3 );
    test.insert( 4 );
    test.insert( 5 );
    test.insert( 6 );
    expected = "[1, 2, 3, 4, 5, 6]";
    evaluateTest( expected , test.toString() , errorMessage );
}

void ArrayListTests::testArrayListClear() {
    string expected;
    string errorMessage = "ArrayList clear() failed!";
    ArrayList<int> test;
    
    //test clearing a cleared list
    test.clear();
    expected = "[]";
    evaluateTest( expected , test.toString(), errorMessage );
    
    //test clearing a cleared list many times
    for ( int i=0 ; i<1000 ; i++ ) {
        test.clear();
    }
    expected = "[]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test clearing a non-empty list
    for ( int i=0 ; i<25 ; i++ ) {
        test.append( i );
    }
    test.clear();
    expected = "[]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test adding to a just-cleared list
    for ( int i=0 ; i<5 ; i++ ) {
        test.append( i );
    }
    expected = "[0, 1, 2, 3, 4]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test clearing a non-empty list many times
    for ( int i=0 ; i<100 ; i++ ) {
        test.append( i );
    }
    for ( int i=0 ; i<10 ; i++ ) {
        test.clear();
    }
    expected = "[]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test appending to the just-cleared list
    for ( int i=0 ; i<4 ; i++ ) {
        test.append( i );
    }
    expected = "[0, 1, 2, 3]";
    evaluateTest( expected , test.toString() , errorMessage );
}

void ArrayListTests::testArrayListInsertAt() {
    string expected;
    string errorMessage = "ArrayList insert() operated incorrectly!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "ArrayList insert() failed to throw proper exception!";
    ArrayList<int> test;
    
    //test inserting at the first index
    for ( int i=10 ; i>=0 ; i-- ) {
        test.insert( 0 , i );
    }
    expected = "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test for insertion out of bounds just outside allowed bounds
    test.clear();
    expectedException = test.generateInsertOutOfBoundsMessage( 1 , 0 );
    foundException = "No exception found";
    try {
        test.insert( 1 , 1 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );

    //test for insertion out of bounds way outside allowed bounds
    test.clear();
    expectedException = test.generateInsertOutOfBoundsMessage( 10001 , 0 );
    foundException = "No exception found";
    try {
        test.insert( 10001 , 1 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test for insertion at negative index
    test.clear();
    expectedException = test.generateInsertOutOfBoundsMessage( -1 , 0 );
    foundException = "No exception found";
    try {
        test.insert( -1 , 10000 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test for insertion at negative index way out of bounds
    test.clear();
    expectedException = test.generateInsertOutOfBoundsMessage( -9999 , 0 );
    foundException = "No exception found";
    try {
        test.insert( -9999 , 12345 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test for insertion out of bounds when we have elements in ArrayList
    test.clear();
    test.append( 0 );
    test.append( 1 );
    test.append( 2 );
    test.append( 3 );
    expectedException = test.generateInsertOutOfBoundsMessage( 5 , 4 );
    foundException = "No exception found";
    try {
        test.insert( 5, 5 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //make sure that list is not altered when exception thrown
    expected = "[0, 1, 2, 3]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    expectedException = test.generateInsertOutOfBoundsMessage( -5 , 4 );
    foundException = "No exception found";
    try {
        test.insert( -5 , 5 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test for insertion on the borders
    test.clear();
    test.append( 0 );
    test.append( 1 );
    test.append( 2 );
    test.append( 3 );
    test.insert( 4 , 4 );
    test.insert( 0 , 0 );
    test.insert( 6 , 6 );
    expected = "[0, 0, 1, 2, 3, 4, 6]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    //test for insertion in the middle
    test.clear();
    test.insert( 0 , 0 );
    test.insert( 1 , 0 );
    test.insert( 1 , 1 );
    expected = "[0, 1, 0]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    test.insert( 2, 1 );
    expected = "[0, 1, 1, 0]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    test.insert( 3 , 3 );
    expected = "[0, 1, 1, 3, 0]";
    evaluateTest( expected , test.toString() , errorMessage );
    
    test.insert( 2, 2 );
    expected = "[0, 1, 2, 1, 3, 0]";
    evaluateTest( expected , test.toString() , errorMessage );
}

void ArrayListTests::testArrayListSet() {
    string expected;
    string found;
    string errorMessage = "ArrayList set() failed to operate correctly!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "ArrayList set() failed to throw proper exception!";
    
    ArrayList<int> test;
    
    //try setting out of bounds on empty list
    test.clear();
    expectedException = test.generateAccessOutOfBoundsMessage( 0 ,  0 );
    foundException = "No exception found";
    try {
        test.set( 0 , 0 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //try setting out of bounds on non-empty list
    test.clear();
    for ( int i=0 ; i<10 ; i++ ) {
        test.append( i );
    }
    expectedException = test.generateAccessOutOfBoundsMessage( 10 , 10 );
    foundException = "No exception found";
    try {
        test.set( 10 , 10 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //make sure that exception does not alter list
    expected = "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //try setting boundaries
    test.set( 0 , 10 );
    expected = "[10, 1, 2, 3, 4, 5, 6, 7, 8, 9]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.set( 9 , 10 );
    expected = "[10, 1, 2, 3, 4, 5, 6, 7, 8, 10]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //try setting middle
    test.set( 4 , 999 );
    expected = "[10, 1, 2, 3, 999, 5, 6, 7, 8, 10]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
}

void ArrayListTests::testArrayListGet() {
    string expected;
    string found;
    string errorMessage = "ArrayList get() failed to operate correctly!";
    
    string expectedRepresentation;
    string foundRepresentation;
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
            "ArrayList Get() failed to throw proper exception!";
    
    ArrayList<int> test;
    
    //test get on boundaries
    test.clear();
    for ( int i=0 ; i<=5 ; i++ ) {
        test.append( i );
    }
    expected = "5";
    found = to_string( test.get( 5 ) );
    evaluateTest( expected , found , errorMessage );
    
    expected = "0";
    found = to_string( test.get( 0 ) );
    evaluateTest( expected , found , errorMessage );
    
    //then test get on middle
    expected = "3";
    found = to_string( test.get( 3 ) );
    evaluateTest( expected , found , errorMessage );
    
    //then test get out of bounds
    expectedException = test.generateAccessOutOfBoundsMessage( 6 , 6 );
    foundException = "No exception found";
    try {
        test.get( 6 );
    }
    catch ( std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( -1 , 6 );
    foundException = "No exception found";
    try {
        test.get( -1 );
    }
    catch ( std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( -9999999 , 6 );
    foundException = "No exception found";
    try {
        test.get( -9999999 );
    }
    catch ( std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //ensure that list is not altered when exception thrown
    expectedRepresentation = "[0, 1, 2, 3, 4, 5]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //test get out of bounds on empty list
    test.clear();
    expectedException = test.generateAccessOutOfBoundsMessage( 0  , 0 );
    foundException = "No exception found";
    try {
        test.get( 0 );
    }
    catch ( std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
}

void ArrayListTests::testArrayListContains() {
    
    bool expected;
    bool found;
    string errorMessage = "ArrayList contains() operated incorrectly!";
    
    ArrayList<int> test;
    
    //test empty list
    test.clear();
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
    
    //test list with even number of elements
    test.clear();
    test.append( 0 );
    test.append( 1 );
    expected = true;
    found = test.contains( 1 );
    evaluateTest( expected , found , errorMessage );
    
    expected = false;
    found = test.contains( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.contains( 0 );
    evaluateTest( expected , found , errorMessage );
    
    //test list with large number of elements
    test.clear();
    for ( int i=0 ; i<1000 ; i++ ) {
        test.append( i );
    }
    
    expected = true;
    found = test.contains( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.contains( 333 );
    evaluateTest( expected , found , errorMessage );
    
    expected = true;
    found = test.contains( 999 );
    evaluateTest( expected , found , errorMessage );
    
    expected = false;
    found = test.contains( 1000 );
    evaluateTest( expected , found , errorMessage );
    
    expected = false;
    found = test.contains( -1 );
    evaluateTest( expected , found , errorMessage );
}

void ArrayListTests::testArrayListIndexOf() {
    int expected;
    int found;
    string errorMessage = "ArrayList indexOf() operated incorrectly!";
    
    ArrayList<int> test;
    
    //test empty list
    test.clear();
    expected = -1;
    found = test.indexOf( 1000 );
    evaluateTest( expected , found , errorMessage );
    
    //test list with 1 element
    test.clear();
    test.append( 0 );
    
    expected = 0;
    found = test.indexOf( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expected = -1;
    found = test.indexOf( 1 );
    evaluateTest( expected , found , errorMessage );
    
    //test list with even number of elements
    test.clear();
    test.append( 1 );
    test.append( 2 );
    test.append( 37 );
    
    expected = 2;
    found = test.indexOf( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expected = 3;
    found = test.indexOf( 37 );
    evaluateTest( expected , found , errorMessage );
    
    expected = -1;
    found = test.indexOf( 4 );
    evaluateTest( expected , found , errorMessage );
    
}

void ArrayListTests::testArrayListRemoveAt() {
    
    int expected;
    int found;
    string expectedRepresentation;
    string foundRepresentation;
    string errorMessage = "ArrayList removeAt() operated incorrectly!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                    "ArrayList removeAt() failed to throw proper exception!";
    
    ArrayList<int> test;
    
    //test remove from empty list
    test.clear();
    expectedException = test.generateAccessOutOfBoundsMessage( 0 , 0 );
    foundException = "No exception found";
    try {
        test.removeAt( 0 );
    }
    catch ( const runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );

    //test removing from list with 1 element
    test.clear();
    test.append( 1000 );
    
    expected = 1000;
    found = test.removeAt( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //test removing from list with 2 elements
    test.clear();
    test.append( 1000 );
    test.append( 123 );
    
    expected = 123;
    found = test.removeAt( 1 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1000]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = 1000;
    found = test.removeAt( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //test removing out of bounds
    test.clear();
    test.append( 945 );
    test.append( 178 );
    test.append( 563 );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 3 , 3 );
    foundException = "No exception found";
    try {
        test.removeAt( 3 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //then test removing from middle
    expected = 178;
    found = test.removeAt( 1 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[945, 563]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //then test removing from previously valid index
    expectedException = test.generateAccessOutOfBoundsMessage( 2 , 2 );
    foundException = "No exception found";
    try {
        test.removeAt( 2 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //then test adding to previously invalid index and then removing
    test.insert( 2 , 135 );
    expected = 135;
    found = test.removeAt( 2 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[945, 563]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //then test removing all by using first index
    expected = 945;
    found = test.removeAt( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[563]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = 563;
    found = test.removeAt( 0 );
    evaluateTest( expected , found , errorMessage );
    expectedRepresentation = "[]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    //then test removing from list that became empty
    expectedException = test.generateAccessOutOfBoundsMessage( 0 , 0 );
    foundException = "No exception found";
    try {
        test.removeAt( 0 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 1 , 0 );
    foundException = "No exception found";
    try {
        test.removeAt( 1 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test removing from middle and then boundaries and then from previously
    //valid index
    test.clear();
    for ( int i=0 ; i<=10 ; i++ ) {
        test.append( i );
    }
    expected = 5;
    found = test.removeAt( 5 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[0, 1, 2, 3, 4, 6, 7, 8, 9, 10]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = 0;
    found = test.removeAt( 0 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1, 2, 3, 4, 6, 7, 8, 9, 10]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = 10;
    found = test.removeAt( 8 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[1, 2, 3, 4, 6, 7, 8, 9]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expectedException = test.generateAccessOutOfBoundsMessage( 8 , 8 );
    foundException = "No exception found";
    try {
        test.removeAt( 8 );
    }
    catch ( const std::runtime_error& e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //ensure that the list is unchanged after an exception is thrown
    expectedRepresentation = "[1, 2, 3, 4, 6, 7, 8, 9]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
}

void ArrayListTests::testArrayListRemove() {
    bool expected;
    bool found;
    string errorMessage = "ArrayList remove() operated incorrectly!";
    
    string expectedRepresentation;
    string foundRepresentation;
    
    ArrayList<int> test;
    
    //test removing from empty list
    test.clear();
    expected = false;
    found = test.remove( 5 );
    evaluateTest( expected , found , errorMessage );
    
    //test removing from list with 1 element
    test.clear();
    test.append( 50 );
    expected = true;
    found = test.remove( 50 );
    evaluateTest( expected , found , errorMessage );
    
    //test removing from list with even number of elements
    test.clear();
    test.append( 50 );
    test.append( 100 );

    expected = false;
    found = test.remove( 75 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[50, 100]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 50 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[100]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
    
    expected = true;
    found = test.remove( 100 );
    evaluateTest( expected , found , errorMessage );
    
    expectedRepresentation = "[]";
    foundRepresentation = test.toString();
    evaluateTest( expectedRepresentation , foundRepresentation , errorMessage );
}

void ArrayListTests::testArrayListResize() {
    
    int expected;
    int found;
    string resizeErrorMessage = "ArrayList does not resize correctly!";
    string sizeErrorMessage = "Incorrect ArrayList size()!";
    
    ArrayList<int> test( 10 );
    
    //test size of list
    expected = 0;
    found = test.size();
    evaluateTest( expected , found , sizeErrorMessage );
    
    expected = 10;
    found = test.m_arraySize;
    evaluateTest( expected , found , resizeErrorMessage );
    
    for ( int i=0 ; i<10 ; i++ ) {
        test.append( i );
    }
    
    expected = 10;
    found = test.size();
    evaluateTest( expected , found , sizeErrorMessage );
    
    expected = 10;
    found = test.m_arraySize;
    evaluateTest( expected , found , resizeErrorMessage );
    
    //test adding additional elements to the list
    //now list should have 11 elements and capacity 22
    test.append( 11 );
    expected = 11;
    found = test.size();
    evaluateTest( expected , found , sizeErrorMessage );
    
    expected = 22;
    found = test.m_arraySize;
    evaluateTest( expected , found , resizeErrorMessage );
    
    //test removing elements from the list to less than half capacity
    for ( int i=0 ; i<6 ; i++ ) {
        test.removeAt( 0 );
    }
    
    //now list should have 5 elements and capacity 10
    expected = 5;
    found = test.size();
    evaluateTest( expected , found , sizeErrorMessage );
    
    expected = 10;
    found = test.m_arraySize;
    evaluateTest( expected , found , resizeErrorMessage );
    
}

void ArrayListTests::testArrayListMemoryUsage() {
    
    cout << "Prepare to observe memory usage. Press enter when ready." << endl;
    getchar();
    cout << "Testing memory usage..." <<
                "Please continue to observe memory usage." << endl;
    ArrayList<int> test;
    for ( int i=0 ; i<300000 ; i++ ) {
        test.append( 100 );
        test.append( 200 );
        test.append( 300 );
        test.append( 400 );
        test.append( 500 );
        test.clear();
    }
    cout << "Was memory usage constant? (y/n)" << endl;
    char result = tolower( getchar() );
    evaluateTest( 'y' , result , "Memory usage by ArrayList is too great.");
}

void ArrayListTests::systemTest() {
    string expected;
    string found;
    string errorMessage = "Failed system test!";
    
    ArrayList<int> test;
    
    //try reversing a list
    test.clear();
    for ( int i=1 ; i<=10 ; i++ ) {
        test.append( i );
    }
    test = reverseList( test );
    expected = "[10, 9, 8, 7, 6, 5, 4, 3, 2, 1]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

ArrayList<int> reverseList( ArrayList<int> l ) {
    for ( int i=0 ; i<l.size()/2 ; i++ ) {
        int tmp = l.get( i );
        l.set( i , l.get( l.size()-1-i ) );
        l.set( l.size()-1-i , tmp );
    }
    return l;
}

