//
//  LinkedStackTests.cpp
//  Data Structures
//
//  Created by Mickey on 10/22/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "LinkedStackTests.h"

LinkedStackTests::LinkedStackTests() {
    
}

void LinkedStackTests::test() {
    testPush();
    testPeek();
    testPop();
    testCopy();
    reportTestStatistics( "LinkedStack" );
}

void LinkedStackTests::testPush() {
    string expected;
    string found;
    string errorMessage = "LinkedStack push() failed!";
    LinkedStack< string > test;
    
    //test empty stack
    test.clear();
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test 1 element
    test.clear();
    test.push( "10" );
    expected = "[10]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test multiple elements
    test.clear();
    test.push( "10" );
    test.push( "11" );
    test.push( "12" );
    expected = "[12, 11, 10]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void LinkedStackTests::testPeek() {
    int expected;
    int found;
    string errorMessage = "LinkedStack peek() failed!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "LinkedStack peek() failed to throw proper exception!";
    
    LinkedStack<int> test;
    
    //test peeking on an empty stack
    test.clear();
    expectedException = test.generateEmptyStackErrorMessage();
    foundException = "No exception found";
    try {
        test.peek();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test peeking on a stack with 1 element
    test.clear();
    test.push( 1 );
    expected = 1;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    //test peeking on a stack with multiple elements
    test.clear();
    test.push( 1 );
    expected = 1;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    test.push( 2 );
    expected = 2;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    
    test.push( 3 );
    expected = 3;
    found = test.peek();
    evaluateTest( expected , found , errorMessage );
    evaluateTest( expected ,  found , errorMessage );
}

void LinkedStackTests::testPop() {
    string expected;
    string found;
    string errorMessage = "LinkedStack pop() failed!";
    
    string expectedException;
    string foundException;
    string exceptionErrorMessage =
                        "LinkedStack pop() fails to throw proper exception!";
    
    LinkedStack<string> test;
    
    //test popping empty stack
    test.clear();
    expectedException = test.generateEmptyStackErrorMessage();
    foundException = "No exception found";
    try {
        test.pop();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //test popping from a stack with 1 element
    test.clear();
    test.push( "1" );
    expected = "1";
    found = test.pop();
    evaluateTest( expected , found , errorMessage );
    
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test popping from a stack with multiple elements
    test.clear();
    test.push( "0" );
    test.push( "1" );
    test.push( "2" );
    test.push( "3" );
    
    expected = "3";
    found = test.pop();
    evaluateTest( expected , found , errorMessage );
    
    expected = "[2, 1, 0]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expected = "2";
    found = test.pop();
    evaluateTest( expected , found , errorMessage );
    
    expected = "[1, 0]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expected = "1";
    found = test.pop();
    evaluateTest( expected , found , errorMessage );
    
    expected = "[0]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expected = "0";
    found = test.pop();
    evaluateTest( expected , found , errorMessage );
    
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedException = test.generateEmptyStackErrorMessage();
    foundException = "No exception found.";
    try {
        test.pop();
    }
    catch ( std::runtime_error e ) {
        foundException = e.what();
    }
    evaluateTest( expectedException , foundException , exceptionErrorMessage );
    
    //make sure causing an empty stack exception does not alter the stack
    expected = "[]";
    found = test.toString();
    evaluateTest( expected , found , exceptionErrorMessage );
}

void LinkedStackTests::testCopy() {
    string errorMessage = "LinkedStack assignment operator failed!";
    LinkedStack<int> test;
    test.push( 0 );
    test.push( 1 );
    test.push( 2 );
    
    LinkedStack<int> copy;
    copy = test;
    
    evaluateTest( test.size() , copy.size() , errorMessage );
    evaluateTest( copy.pop() , test.pop() , errorMessage );
    
    evaluateTest( test.size() , copy.size() , errorMessage );
    evaluateTest( copy.pop() , test.pop() , errorMessage );
    
    evaluateTest( test.size() , copy.size() , errorMessage );
    evaluateTest( copy.pop() , test.pop() , errorMessage );
}